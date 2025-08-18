#include <QDateTime>
#include <QDebug>

#include "todoitem.h"

TodoItem::TodoItem(QObject *parent)
    : QObject{parent}
{}

bool TodoItem::done() const
{
    return _done;
}
QString TodoItem::description() const
{
    return _description;
}
long long TodoItem::timeEstimate() const
{
    return _timeEstimate;
}
long long TodoItem::timeElapsed() const
{
    return _timeElapsed;
}
long long TodoItem::timeRemaining() const
{
    return _timeEstimate - _timeElapsed;
}
Blocklist *TodoItem::blocklist() const
{
    return _blocklist;
}

void TodoItem::setDone(bool value)
{
    _done = value;
    emit doneChanged();
}

void TodoItem::setDescription(const QString &value)
{
    _description = value;
    emit descriptionChanged();
}

void TodoItem::setTimeEstimate(long long value)
{
    _timeEstimate = value;
    emit timeEstimateChanged();
    emit timeRemainingChanged();
}

void TodoItem::setTimeElapsed(long long value)
{
    _timeElapsed = value;
    resetTimer();
    emit timeElapsedChanged();
    emit timeRemainingChanged();
}
void TodoItem::setBlocklist(Blocklist *value)
{
    if (_blocklist) {
        _blocklist->disconnect(this);
        if (_watching)
            _blocklist->setWatching(false);
    }
    _blocklist = value;
    if (_blocklist) {
        connect(_blocklist, &Blocklist::destroyed, this, [this] {
            _blocklist = nullptr;
            emit blocklistChanged();
        });
        if (_watching)
            _blocklist->setWatching(true);
    }
    if (_watching)
        applyBlocklist();
    emit blocklistChanged();
}

void TodoItem::setWatching(bool value)
{
    _watching = value;
    if (_blocklist) {
        _blocklist->setWatching(value);
    }
}

// Call before connecting the timer
void TodoItem::resetTimer()
{
    _lastResetTime = QDateTime::currentMSecsSinceEpoch() - _timeElapsed;
}

void TodoItem::updateTimer()
{
    _timeElapsed = QDateTime::currentMSecsSinceEpoch() - _lastResetTime;
    emit timeElapsedChanged();
    emit timeRemainingChanged();
    timerExpired(); // performs check
}

bool TodoItem::timerExpired()
{
    bool oldTimerExpired = _timerExpired;
    _timerExpired = _timeElapsed >= _timeEstimate;
    if (oldTimerExpired != _timerExpired)
        emit timerExpiredChanged();
    return _timerExpired;
}

bool TodoItem::applyBlocklist()
{
    if (_blocklist == nullptr) {
        Blocklist::removeAllBlocks();
        return false;
    }
    _blocklist->applyBlocks();
    return true;
}
