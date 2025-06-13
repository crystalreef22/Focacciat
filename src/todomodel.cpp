#include "todomodel.h"
#include <QDebug>
#include "todolist.h"

TodoModel::TodoModel(QObject *parent)
    : QAbstractListModel(parent)
    , _list(nullptr)
    , _activeItem()
{}

int TodoModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !_list)
        return 0;

    return _list->items().size();
}

QVariant TodoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !_list)
        return QVariant();

    const TodoItem item = _list->items().at(index.row());
    switch (role) {
    case DoneRole:
        return QVariant(item.done);
    case DescriptionRole:
        return QVariant(item.description);
    case TimeEstimateRole:
        return QVariant(item.timeEstimate);
    case TimeElapsedRole:
        return QVariant(item.timeElapsed);
    case ActiveRole:
        return index == _activeItem;
    }

    return QVariant();
}

bool TodoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!_list) return false;

    TodoItem item = _list->items().at(index.row());
    switch (role) {
    case DoneRole:
        item.done = value.toBool();
        break;
    case DescriptionRole:
        item.description = value.toString();
        break;
    case TimeEstimateRole:
        item.timeEstimate = value.toLongLong();
        break;
    case TimeElapsedRole:
        return false;
    case ActiveRole:
        if (value.toBool()) {
            const QModelIndex oldActive = _activeItem;
            _activeItem = TodoModel::createIndex(index.row(), 0);
            if (oldActive.isValid()) {
                emit dataChanged(oldActive, oldActive, {ActiveRole});
            }
        } else {
            _activeItem = QModelIndex();
        }
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
    names[TimeElapsedRole] = "timeElapsed";
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
            const int index = _list->items().size();
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
    }
    endResetModel();
}
