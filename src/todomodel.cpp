#include "todomodel.h"
#include <QDebug>

TodoModel::TodoModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int TodoModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return _list.size();
}

QVariant TodoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const TodoItem* const item = _list.at(index.row());
    switch (role) {
    case DoneRole:
        return QVariant(item->done);
    case DescriptionRole:
        return QVariant(item->description);
    case TimeEstimateRole:
        return QVariant(item->timeEstimate);
    case TimeRemainingRole:
        return QVariant(item->timeEstimate - item->timeElapsed);
    case TimeElapsedRole:
        return QVariant(item->timeElapsed);
    case ActiveRole:
        return _activeIndex == index;
    }

    return QVariant();
}

bool TodoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    TodoItem*const item = _list.at(index.row());
    switch (role) {
    case DoneRole:
        item->done = value.toBool();
        break;
    case DescriptionRole:
        item->description = value.toString();
        break;
    case TimeEstimateRole:
        item->timeEstimate = value.toLongLong();
        emit dataChanged(index, index, {role});
        emit dataChanged(index, index, {TimeRemainingRole});
        return true; //todo
    case TimeRemainingRole:
        qWarning() << "todomodel.cpp: tried to time travel";
        return false;
    case TimeElapsedRole:
        item->timeElapsed = value.toLongLong();
        // item->updateTimeElapsed(); // makes it immediately update time
        break;
    case ActiveRole:
        if (_activeIndex == index) {
            _activeIndex = QModelIndex{};
            return true;
        }
        const auto oldIndex = _activeIndex;
        _activeIndex = index;
        emit dataChanged(oldIndex, oldIndex, {ActiveRole});
        emit dataChanged(index, index, {ActiveRole});
        return true;
    }
    return true;
    // TODO: make it return based on whether changed
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
    names[DoneRole] = "done";
    names[DescriptionRole] = "description";
    names[TimeEstimateRole] = "timeEstimate";
    names[TimeRemainingRole] = "timeRemaining";
    names[TimeElapsedRole] = "timeElapsed";
    names[ActiveRole] = "active";
    return names;
}

void TodoModel::appendItem() {
    const int index = _list.size();
    beginInsertRows(QModelIndex{}, index, index);
    _list.append(new TodoItem{});
    endInsertRows();
}
void TodoModel::removeCompletedItems()
{
    for (qsizetype i = 0; i < _list.size();) {
        if (_list.at(i)->done) {
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

#if false
void TodoModel::setList(TodoList *list)
{
    beginResetModel();
    if (_list) {
        list->disconnect(this);
    }
    _list = list;

    if (_list) {
        connect(_list, &TodoList::preItemAppended, this, [=]() {
            const int index = _list->getItems().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(_list, &TodoList::postItemAppended, this, [=]() {
            endInsertRows();
        });
        connect(_list, &TodoList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(_list, &TodoList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });
        connect(_list, &TodoList::timeElapsedUpdated, this, [=](int index) {
            const auto modelIdx = TodoModel::createIndex(index, 0);
            emit dataChanged(modelIdx, modelIdx, {TimeRemainingRole});
        });
    }
    endResetModel();
}

#endif
