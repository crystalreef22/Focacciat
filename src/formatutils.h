#ifndef FORMATUTILS_H
#define FORMATUTILS_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

class FormatUtils : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit FormatUtils(QObject *parent = nullptr);

    Q_INVOKABLE static QString msToTime(long long ms);
signals:
};

#endif // FORMATUTILS_H
