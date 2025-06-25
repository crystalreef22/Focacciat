#ifndef FORMATUTILS_H
#define FORMATUTILS_H

#include <QObject>

class FormatUtils : public QObject {
    Q_OBJECT
public:
    explicit FormatUtils(QObject *parent = nullptr);

    Q_INVOKABLE static QString msToTime(long long ms);

signals:
};

#endif // FORMATUTILS_H
