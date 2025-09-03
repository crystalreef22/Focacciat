#include "todomodel.h"
#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include "blockers/blocklist.h"

TodoModel::TodoModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_timer.setInterval(100);
    m_timer.setTimerType(Qt::TimerType::PreciseTimer);
    m_timer.start();
    resetPausedTime();
    connect(&m_timer, &QTimer::timeout, this, &TodoModel::updatePausedTime);
}

int TodoModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_list.size();
}

QVariant TodoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TodoItem *const item = m_list.at(index.row());
    switch (role) {
    case ItemRole:
        return QVariant::fromValue(item);
    case ActiveRole:
        return m_activeIndex == index;
    }

    return QVariant();
}

bool TodoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    TodoItem *const item = m_list.at(index.row());
    switch (role) {
    case ItemRole:
        return false;
    case ActiveRole:
        TodoItem *oldItem = activeItem();
        if (oldItem) {
            oldItem->setWatching(false);
            m_timer.disconnect(oldItem);
        }
        m_paused = false;
        emit pausedChanged();
        resetPausedTime();
        if (m_activeIndex == index) {
            m_activeIndex = QPersistentModelIndex{};
            emit activeItemChanged();
            Blocklist::removeAllBlocks();
            return true;
        }
        const QModelIndex oldIndex = m_activeIndex;
        m_activeIndex = index;
        item->resetTimer();
        item->applyBlocklist();
        item->setWatching(true);
        connect(&m_timer, &QTimer::timeout, item, &TodoItem::updateTimer);
        emit activeItemChanged();
        emit dataChanged(oldIndex, oldIndex, {ActiveRole});
        emit dataChanged(index, index, {ActiveRole});
        return true;
    }
    return false;
}

Qt::ItemFlags TodoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    // in tutorial it was just return Qt::ItemIsEditable
}

QHash<int, QByteArray> TodoModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[ItemRole] = "item";
    names[ActiveRole] = "active";
    return names;
}

void TodoModel::appendItem()
{
    const int index = m_list.size();
    beginInsertRows(QModelIndex{}, index, index);
    m_list.append(new TodoItem(this));
    endInsertRows();
}

void TodoModel::removeCompletedItems()
{
    // make sure to clear active index if removed
    if (m_activeIndex.isValid() && m_list.at(m_activeIndex.row())->done()) {
        m_activeIndex = QPersistentModelIndex{};
        resetPausedTime();
        m_paused = false;
        emit pausedChanged();
        emit activeItemChanged();
        Blocklist::removeAllBlocks();
    }

    for (qsizetype i = 0; i < m_list.size();) {
        if (m_list.at(i)->done()) {
            beginRemoveRows(QModelIndex{}, i, i);
            m_list.at(i)->deleteLater();
            m_list.removeAt(i);
            endRemoveRows();
        } else {
            ++i;
        }
    }
}

bool TodoModel::moveItem(int fromIndex, int toIndex)
{
    if (fromIndex >= m_list.length() || toIndex >= m_list.length() || fromIndex < 0 || toIndex < 0
        || fromIndex == toIndex)
        return false;
    beginMoveRows(QModelIndex{},
                  fromIndex,
                  fromIndex,
                  QModelIndex{},
                  toIndex + (toIndex > fromIndex));
    // see https://doc.qt.io/qt-6/qabstractitemmodel.html#beginMoveRows ~~~~~~~ ^
    m_list.move(fromIndex, toIndex);
    endMoveRows();
    return true;
}

long long TodoModel::pausedTime()
{
    return m_pausedTime;
}

void TodoModel::setPausedTime(long long value) {
    m_pausedTime = value;
    m_pausedLastResetTime = QDateTime::currentMSecsSinceEpoch() - m_pausedTime;
    emit pausedTimeChanged();
}

QJsonObject TodoModel::serialize() const {
    QJsonArray todoItems{};
    for (const TodoItem * item : std::as_const(m_list)) {
        todoItems.append(item->serialize());
    }
    // TODO: serialize active index
    QJsonObject result{
        {"todoItems", todoItems}
    };
    return result;
}
void TodoModel::deserialize(const QJsonObject& json) {
    TodoItem* item = activeItem();
    Blocklist::removeAllBlocks();
    if (item) {
        item->setWatching(false);
        // may not be needed, but
        m_timer.disconnect(item);
    }
    const QJsonArray& todoItemsJson(json["todoItems"].toArray());
    // FIXME: does not check if json is proper
    // clear all list items
    m_activeIndex = QModelIndex{};
    emit activeItemChanged();
    setPaused(false);
    resetPausedTime();
    beginResetModel();
    for (TodoItem* listItem : std::as_const(m_list)) {
        listItem->deleteLater();
    }
    m_list = {};
    m_list.reserve(todoItemsJson.size());
    for (const QJsonValue& todoItemJson : todoItemsJson) {
        m_list.append(TodoItem::deserialize(todoItemJson.toObject(), this));
    }
    endResetModel();
    // TODO: serialize active index
}

void TodoModel::updatePausedTime()
{
    m_pausedTime = QDateTime::currentMSecsSinceEpoch() - m_pausedLastResetTime;
    emit pausedTimeChanged();
}

void TodoModel::resetPausedTime()
{
    m_pausedLastResetTime = QDateTime::currentMSecsSinceEpoch();
    m_pausedTime = 0;
    emit pausedTimeChanged();
}

TodoItem *TodoModel::activeItem() const
{
    if (m_activeIndex.isValid())
        return m_list.at(m_activeIndex.row());
    else
        return nullptr;
}

bool TodoModel::paused() const
{
    return m_paused;
}

bool TodoModel::setPaused(bool value)
{
    if (m_paused == value)
        return false;
    TodoItem *item = activeItem();
    if (!item)
        return false;

    m_paused = value;
    if (m_paused) {
        m_timer.disconnect(item);
        resetPausedTime();
    } else {
        item->resetTimer();
        resetPausedTime();
        connect(&m_timer, &QTimer::timeout, item, &TodoItem::updateTimer);
    }
    emit pausedChanged();

    return true;
}
