#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "globalstate.h"
#include "extensionclient.h"

ExtensionClient::ExtensionClient(QLocalSocket* conn, QObject *parent)
    : m_connection{conn}
    , QObject{parent}
{
    connect(conn, &QLocalSocket::disconnected, this, &ExtensionClient::disconnected);
    connect(conn, &QLocalSocket::readyRead, this, &ExtensionClient::readMessage);
}

void ExtensionClient::readMessage() {
    QByteArray data{m_connection->readAll()};
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "recieved errored data: " << data;
    } else {
        QJsonObject obj = doc.object();
        QJsonObject request = obj.value("request").toObject();
        if (!request.empty()) {
            QString type = request.value("type").toString();
            if (type == "blocklist") {
                qInfo() << "Sending blocklist to one";
                GlobalState::instance()->blocklistListModel()->getActiveItem
            } else {
                qWarning() << "Recieved unknown request for " << type;
            }
        }
    }
}
