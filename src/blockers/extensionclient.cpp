#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "globalstate.h"
#include "extensionclient.h"
#include "blocklist.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ExtensionClient::ExtensionClient(QLocalSocket* conn, QObject *parent)
    : m_connection{conn}
    , QObject{parent}
{
    connect(conn, &QLocalSocket::disconnected, this, &ExtensionClient::disconnected);
    connect(conn, &QLocalSocket::readyRead, this, &ExtensionClient::readMessage);
}

bool ExtensionClient::sendBlocklist(QLocalSocket* client) {
    Blocklist* blocklist = GlobalState::instance()->blocklistListModel()->activeItem();
    QJsonDocument obj {
        QJsonObject{
            {"response", QJsonObject{
                {"type", "blocklist"},
                {"name", blocklist->name()},
                {"data", QJsonArray::fromStringList(blocklist->websiteList().split("\n"))}
            }}
        }
    };
    return sendRaw(obj.toJson(QJsonDocument::Compact));
}

bool ExtensionClient::sendRaw(const QByteArray& bytes) {
    // TODO: this is a mess
    bool success{false};
    uint32_t header = bytes.size();
    success |= (m_connection->write(reinterpret_cast<char*>(&header), sizeof(header)) == sizeof(header));
    success |= (m_connection->write(bytes) == bytes.length());
    if (!success) qInfo() << "did not write to any clients";
    return success;
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

            } else {
                qWarning() << "Recieved unknown request for " << type;
            }
        }
    }
}
