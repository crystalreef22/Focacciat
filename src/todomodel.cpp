#include "todomodel.h"
#include <QDebug>
#include "todolist.h"

TodoModel::TodoModel(QObject *parent)
    : QAbstractListModel(parent)
    , _list(nullptr)
{ }

int TodoModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !_list)
        return 0;

    return _list->getItems().size();
}

QVariant TodoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !_list)
        return QVariant();

    const TodoItem item = _list->getItems().at(index.row());
    switch (role) {
    case DoneRole:
        return QVariant(item.done);
    case DescriptionRole:
        return QVariant(item.description);
    case TimeEstimateRole:
        return QVariant(item.timeEstimate);
    case TimeRemainingRole:
        return QVariant(item.timeEstimate - item.timeElapsed);
    case ActiveRole:
        return _list->getActiveIndex() == index.row();
    }

    return QVariant();
}

bool TodoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!_list) return false;

    TodoItem item = _list->getItems().at(index.row());
    switch (role) {
    case DoneRole:
        item.done = value.toBool();
        break;
    case DescriptionRole:
        item.description = value.toString();
        break;
    case TimeEstimateRole:
        item.timeEstimate = value.toLongLong();
        if (_list->setItemAt(index.row(), item)) {
            emit dataChanged(index, index, {role});
            emit dataChanged(index, index, {TimeRemainingRole});
            return true;
        }
        return false;
    case TimeRemainingRole:
        qWarning() << "todomodel.cpp: tried to time travel";
        return false;
    case ActiveRole:
        if (_list->getActiveIndex() == index.row()) {
            _list->setActiveIndex(-1);
            return true;
        }
        const auto oldIndex = TodoModel::createIndex(_list->getActiveIndex(), 0);
        _list->setActiveIndex(index.row());
        emit dataChanged(oldIndex, oldIndex, {ActiveRole});
        emit dataChanged(index, index, {ActiveRole});
        return true;
    }

    if (_list->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, {role});
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
    names[DoneRole] = "done";
    names[DescriptionRole] = "description";
    names[TimeEstimateRole] = "timeEstimate";
    names[TimeRemainingRole] = "timeRemaining";
    names[ActiveRole] = "active";
    return names;
}

TodoList *TodoModel::list() const
{
    return _list;
}

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
