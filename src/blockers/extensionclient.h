#ifndef EXTENSIONCLIENT_H
#define EXTENSIONCLIENT_H

#include <QObject>

class extensionClient : public QObject
{
    Q_OBJECT
public:
    explicit extensionClient(QObject *parent = nullptr);

signals:
};

#endif // EXTENSIONCLIENT_H
