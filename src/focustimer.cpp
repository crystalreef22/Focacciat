#include "focustimer.h"

#include <QDebug>
#include <QDateTime>


FocusTimer::FocusTimer(QTimer *timer, QObject *parent)
{
    timer = timer;
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, &QTimer::timeout, this, &FocusTimer::updateTimer);
}

void FocusTimer::updateTimer() {
    const long long currentMs = QDateTime::currentMSecsSinceEpoch();
    const long long elapsedMs = _elapsedMsSinceEpoch - currentMs;
    emit timerUpdated(elapsedMs);
}

void FocusTimer::start(long long timerLength) {
    _elapsedMsSinceEpoch = QDateTime::currentMSecsSinceEpoch() + timerLength;
    timer->start(100);
    updateTimer();
}
