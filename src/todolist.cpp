#include "todolist.h"

TodoList::TodoList(QObject *parent)
    : QObject{parent}
{
    //_items.append( {true, QStringLiteral("testtest1") });
}

QVector<TodoItem> TodoList::items() const
{
    return _items;
}

bool TodoList::setItemAt(int index, const TodoItem &item)
{
    if (index < 0 || index >= _items.size()) return false;
    const TodoItem &oldItem = _items.at(index);
    if (item.done == oldItem.done && item.description == oldItem.description) {
        return false;
    }

    _items[index] = item;
    return true;
}

void TodoList::appendItem()
{
    emit preItemAppended();

    TodoItem item;
    item.done = false;
    _items.append(item);

    emit postItemAppended();
}

void TodoList::removeCompletedItems()
{
    for (int i = 0; i < _items.size(); ) {
        if (_items.at(i).done) {
            emit preItemRemoved(i);

            _items.removeAt(i);

            emit postItemRemoved();
        } else {
            i++;
        }
    }
}
