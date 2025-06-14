#include "todolist.h"
#include <QDateTime>

TodoList::TodoList(QObject *parent)
    : QObject{parent}
{
    //_items.append( {true, QStringLiteral("testtest1") });
    connect(&_timer, &QTimer::timeout, this, &TodoList::updateTimeElapsed);
}

// private
void TodoList::removeItem(qsizetype index) {
    _items.removeAt(index);
    if (_activeIndex > index) --_activeIndex;
    else if (_activeIndex == index) _activeIndex = -1;
}

// public

const QVector<TodoItem>& TodoList::getItems() const
{
    return _items;
}


// precondition: an item is active
const TodoItem& TodoList::getActiveItem() const {
    return _items.at(_activeIndex);
}

bool TodoList::activeItemExists() const {
    return _activeIndex >= 0 && _activeIndex < _items.size();
}

qsizetype TodoList::getActiveIndex() const {
    return _activeIndex;
}

void TodoList::setActiveIndex(qsizetype index) {
    long long startTime{0};
    if (activeItemExists()) {
        startTime = getActiveItem().timeElapsed;
    }
    _activeIndex = index;
    _lastStartEpoch = QDateTime::currentMSecsSinceEpoch() - startTime;
    _timer.start(100);
}

bool TodoList::setItemAt(qsizetype index, const TodoItem &item)
{
    if (index < 0 || index >= _items.size()) return false;
    const TodoItem &oldItem = _items.at(index);
    if (item.done == oldItem.done && item.description == oldItem.description &&
            item.timeElapsed == oldItem.timeElapsed
        && item.timeEstimate == oldItem.timeEstimate ) {
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
    item.timeEstimate = 0;
    item.timeElapsed = 0;
    _items.append(item);

    emit postItemAppended();
}

void TodoList::removeCompletedItems()
{
    for (qsizetype i = 0; i < _items.size(); ) {
        if (_items.at(i).done) {
            emit preItemRemoved(i);

            removeItem(i);

            emit postItemRemoved();
        } else {
            i++;
        }
    }
}

void TodoList::updateTimeElapsed() {
    if (activeItemExists()) {
        _items[_activeIndex].timeElapsed = QDateTime::currentMSecsSinceEpoch() - _lastStartEpoch;
        emit timeElapsedUpdated(_activeIndex);
        qInfo() << _items[_activeIndex].timeElapsed;
    }
}
