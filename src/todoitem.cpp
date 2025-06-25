#include <QDateTime>

#include "todoitem.h"

TodoItem::TodoItem(QObject *parent)
    : QObject{parent}
{ }

bool TodoItem::done() const { return _done; }
QString TodoItem::description() const { return _description; }
long long TodoItem::timeEstimate() const { return _timeEstimate; }
long long TodoItem::timeElapsed() const { return _timeElapsed; }
long long TodoItem::timeLeft() const {
    return _timeEstimate - _timeElapsed;
}

void TodoItem::setDone(bool value) {
    _done = value;
    emit doneChanged();
}

void TodoItem::setDescription(QString value){
    _description = value;
    emit descriptionChanged();
}

void TodoItem::setTimeEstimate(long long value){
    _timeEstimate = value;
    emit timeEstimateChanged();
    emit timeElapsedChanged();
}

void TodoItem::setTimeElapsed(long long value){
    _timeElapsed = value;
    resetTimer();
    emit timeEstimateChanged();
    emit timeElapsedChanged();
}

void TodoItem::resetTimer() {
    _lastResetTime = QDateTime::currentMSecsSinceEpoch() - _timeElapsed;
}

void TodoItem::updateTimer() {
    _timeElapsed = QDateTime::currentMSecsSinceEpoch() - _lastResetTime;
}

