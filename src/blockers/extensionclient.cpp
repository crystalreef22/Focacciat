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

void ExtensionClient::requestPing() const {
    sendJson(QJsonDocument{
        QJsonObject{
            {"type", "response"},
            {"action", "ping"}
        }
    });
}

bool ExtensionClient::sendBlocklist() const {
    const Blocklist* blocklist = GlobalState::instance()->blocklistManager()->activeItem();
    // FIXME: blocklist may be nullptr!!!
    QJsonDocument obj {
        QJsonObject{
            {"type", "response"},
            {"action", "blocklist.get"},
            {"name", blocklist->name()},
            {"data", QJsonArray::fromStringList(blocklist->websiteList().split("\n"))}
        }
    };
    return sendJson(obj);
}

bool ExtensionClient::sendJson(const QJsonDocument& doc) const {
    auto bytes = doc.toJson(QJsonDocument::Compact);
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
    const QJsonDocument& doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "recieved errored data: " << data;
    } else {
        const QJsonObject& obj = doc.object();
        const QString& type = obj.value("type").toString();
        const QString& action = obj.value("action").toString();
        if (type == "request") {
            if (action == "blocklist.get") {
                sendBlocklist();
            } else if (action == "blocklist.append") {
                if (!obj.value("data").isArray()) [[unlikely]] {
                    sendJson(QJsonDocument{
                        QJsonObject{
                            {"type","response"},
                            {"action","blocklist.append"},
                            {"error",
                                QJsonObject{{"code", "invalid_data"}}
                            }
                        }
                    });
                } else {
                    const QJsonArray& arr = obj.value("data").toArray();
                    Blocklist* blocklist = GlobalState::instance()->blocklistManager()->activeItem();
                    // FIXME: blocklist may be nullptr!!!
                    for (const QJsonValue& item : arr) {
                        blocklist->appendWebsites(item.toString());
                    }
                }
            } else if (action == "ping") {
                sendJson(QJsonDocument{
                    QJsonObject{
                        {"type", "response"},
                        {"action", "ping"}
                    }
                });
            } else {
                qWarning() << "unhandled action of " << obj.value("action");
            }
        } else if (type == "response") {
            if (action == "ping") {
                emit pingRecieved();
            }
        } else [[unlikely]] {
            qWarning() << "unhandled message recieved of type " << obj.value("type");
        }
    }
}
