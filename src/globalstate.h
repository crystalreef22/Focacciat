#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H

#include <QObject>
#include <QQmlEngine>

class GlobalState : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit GlobalState(QObject *parent = nullptr);

signals:
};

#endif // GLOBALSTATE_H
