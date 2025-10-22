#include <QDateTime>
#include <QDebug>

#include "todoitem.h"
#include <QJsonObject>

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
    }
    _blocklist = value;
    if (_blocklist) {
        connect(_blocklist, &Blocklist::destroyed, this, [this] {
            _blocklist = nullptr;
            emit blocklistChanged();
        });
    }
    emit blocklistChanged();
}

QJsonObject TodoItem::serialize() const {
    return QJsonObject{
        { "done", _done },
        { "description", _description },
        { "timeEstimate", _timeEstimate },
        { "timeElapsed", _timeElapsed },
        { "blocklistUUID", _blocklist ? QJsonValue(_blocklist->UUID().toString()) : QJsonValue(QJsonValue::Null) }
    };
}

TodoItem* TodoItem::deserialize(const QJsonObject& json, QObject *parent) {
    TodoItem* item = new TodoItem(parent);
    item->_done = json.value("done").toBool(false);
    item->_description = json.value("description").toString("");
    item->_timeEstimate = json.value("timeEstimate").toVariant().toLongLong();
    item->_timeElapsed = json.value("timeElapsed").toVariant().toLongLong();
    const auto blocklistUUID = QUuid::fromString(json.value("blocklistUUID").toString());
    if (blocklistUUID.isNull()) {
        item->setBlocklist(Blocklist::fromUUID(QUuid::fromString(json.value("blocklistUUID").toString())));
    }
    return item;
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
