#ifndef EXTENSIONCLIENT_H
#define EXTENSIONCLIENT_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>
#include <QLocalSocket>
#include <QLocalServer>

class ExtensionClient : public QObject
{
    Q_OBJECT
public:
    explicit ExtensionClient(QLocalSocket* client, QObject *parent = nullptr);

signals:
    void disconnected();

private:
    void readMessage();

    QLocalSocket* m_connection;
    int m_connectedver;
    bool sendBlocklist(QLocalSocket *client);
    bool sendRaw(const QByteArray &bytes);
};

#endif // EXTENSIONCLIENT_H
