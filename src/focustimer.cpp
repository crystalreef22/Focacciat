#include "focustimer.h"

#include <QDebug>
#include <QDateTime>


FocusTimer::FocusTimer(QObject *parent)
    : QTimer{parent}
{
    setTimerType(Qt::PreciseTimer);
    connect(this, &QTimer::timeout, this, &FocusTimer::updateTimer);
}

QString FocusTimer::toString() {
    return QString::number(_elapsedSecsSinceEpoch);
}


void FocusTimer::updateTimer() {

}
