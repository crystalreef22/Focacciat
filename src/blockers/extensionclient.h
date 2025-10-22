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

    void requestPing() const;

signals:
    void disconnected();
    void pingRecieved();

private:
    void readMessage();

    QLocalSocket* m_connection;
    int m_connectedver;
    bool sendBlocklist() const;
    bool sendJson(const QJsonDocument& doc) const;

    bool sendError(const QString &errorcode, const QString &description = "") const;
};

#endif // EXTENSIONCLIENT_H
