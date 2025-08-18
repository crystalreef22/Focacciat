#include "formatutils.h"

FormatUtils::FormatUtils(QObject *parent)
    : QObject{parent}
{}

QString FormatUtils::msToTime(long long ms)
{
    QString negative = ms >= -1000 ? "" : "\u2212";
    long long totalSecs = abs((ms + (ms > 0) * 999) / 1000);

    long long s = totalSecs % 60;
    long long m = (totalSecs / 60) % 60;
    long long h = (totalSecs / 60 / 60);
    return negative
           + QString("%1:%2:%3")
                 .arg(h, 2, 10, QChar('0'))
                 .arg(m, 2, 10, QChar('0'))
                 .arg(s, 2, 10, QChar('0'));
}
