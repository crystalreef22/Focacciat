#include "formatutils.h"

FormatUtils::FormatUtils(QObject *parent)
    : QObject{parent}
{}

QString FormatUtils::msToTime(long long ms) {
    return QString::number(ms);
}
