#include "focustimer.h"

#include <QDebug>
#include <QDateTime>


FocusTimer::FocusTimer(QObject *parent)
    : QTimer{parent}
{
    setTimerType(Qt::PreciseTimer);
    connect(this, &QTimer::timeout, this, &FocusTimer::updateTimer);
}

FocusTimer::FocusTimer(long long timerLength)
{
    _elapsedMsSinceEpoch = QDateTime::currentMSecsSinceEpoch() + timerLength;
    setTimerType(Qt::PreciseTimer);
    connect(this, &QTimer::timeout, this, &FocusTimer::updateTimer);
}

void FocusTimer::updateTimer() {
    const long long currentMs = QDateTime::currentMSecsSinceEpoch();
    const long long elapsedMs = _elapsedMsSinceEpoch - currentMs;
    emit timerUpdated(elapsedMs);
    qDebug() << elapsedMs;
}
