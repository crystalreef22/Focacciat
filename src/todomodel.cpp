#include "todomodel.h"
#include <QDebug>
#include "blocklist.h"

TodoModel::TodoModel(QObject *parent)
    : QAbstractListModel(parent)
{
    _timer.setInterval(100);
    _timer.setTimerType(Qt::TimerType::PreciseTimer);
    _timer.start();
}

int TodoModel::rowCount(const QModelIndex &parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return _list.size();
}

QVariant TodoModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    TodoItem* const item = _list.at(index.row());
    switch (role) {
    case ItemRole:
        return QVariant::fromValue(item);
    case ActiveRole:
        return _activeIndex == index;
    }

    return QVariant();
}

bool TodoModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    TodoItem*const item = _list.at(index.row());
    switch (role) {
    case ItemRole:
        return false;
    case ActiveRole:
        _timer.disconnect();
        TodoItem* oldItem = activeItem();
        if (oldItem) oldItem->setWatching(false);
        if (_activeIndex == index) {
            _activeIndex = QPersistentModelIndex{};
            emit activeItemChanged();
            Blocklist::removeAllBlocks();
            return true;
        }
        const QModelIndex oldIndex = _activeIndex;
        _activeIndex = index;
        item->resetTimer();
        item->applyBlocklist();
        item->setWatching(true);
        connect(&_timer, &QTimer::timeout, item, &TodoItem::updateTimer);
        emit activeItemChanged();
        emit dataChanged(oldIndex, oldIndex, {ActiveRole});
        emit dataChanged(index, index, {ActiveRole});
        return true;
    }
    return false;
}

Qt::ItemFlags TodoModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    // in tutorial it was just return Qt::ItemIsEditable
}

QHash<int, QByteArray> TodoModel::roleNames() const {
    QHash<int, QByteArray> names;
    names[ItemRole] = "item";
    names[ActiveRole] = "active";
    return names;
}

void TodoModel::appendItem() {
    const int index = _list.size();
    beginInsertRows(QModelIndex{}, index, index);
    _list.append(new TodoItem(this));
    endInsertRows();
}

void TodoModel::removeCompletedItems() {
    // make sure to clear active index if removed
    if (_activeIndex.isValid() && _list.at(_activeIndex.row())->done()) {
        _activeIndex = QPersistentModelIndex{};
        emit activeItemChanged();
    }

    for (qsizetype i = 0; i < _list.size();) {
        if (_list.at(i)->done()) {
            beginRemoveRows(QModelIndex{}, i, i);
            _list.at(i)->deleteLater();
            _list.removeAt(i);
            endRemoveRows();
        }
        else {
            ++i;
        }
    }
}

TodoItem* TodoModel::activeItem() const {
    if (_activeIndex.isValid())
        return _list.at(_activeIndex.row());
    else
        return nullptr;
}
