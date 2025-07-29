#include "extensionintegration.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QLocalServer>
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

ExtensionIntegration::ExtensionIntegration(QObject *parent)
    : QObject{parent}
{
    ExtensionIntegration::m_pThis = this;

    m_firefoxNMManifestDir = QDir::homePath() + "/Library/Application Support/Mozilla/NativeMessagingHosts";

    qWarning() << "see https://doc.qt.io/qt-6/qlocalserver.html#removeServer";
    QLocalServer::removeServer(m_serverName);

    m_server.setSocketOptions(QLocalServer::UserAccessOption);
    m_server.listen(m_serverName);

    qInfo() << "listening on " << m_server.fullServerName();
    connect(&m_server, &QLocalServer::newConnection, this, &ExtensionIntegration::connectNextSocket);
}

ExtensionIntegration* ExtensionIntegration::instance() {
    if (m_pThis == nullptr) // avoid creation of new instances
        m_pThis = new ExtensionIntegration;
    return ExtensionIntegration::m_pThis;
}
ExtensionIntegration* ExtensionIntegration::create(QQmlEngine *engine, QJSEngine *scriptEngine) {
    return ExtensionIntegration::instance();
}

void ExtensionIntegration::connectNextSocket() {
    QLocalSocket* conn = m_server.nextPendingConnection();
    if (!conn) {
        qWarning() << "connection failed";
        return;
    }
    if (!conn->isValid()) {
        qWarning() << "connection invalid";
        conn->deleteLater();
        return;
    }
    m_clients.append(conn);
    connect(conn, &QLocalSocket::disconnected, this, &ExtensionIntegration::socketDisconnected);
    connect(conn, &QLocalSocket::readyRead, this, [=]{
        readMessage(conn);
    });
}

void ExtensionIntegration::socketDisconnected() {
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    if (socket) {
        m_clients.removeOne(socket);
        socket->deleteLater();
    }
}


void ExtensionIntegration::readMessage(QLocalSocket* conn) {
    // header is taken care of by extensionhost
    QByteArray data{conn->readAll()};
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "recieved errored data: " << data;
    } else {
        QJsonObject obj = doc.object();
        QJsonObject request = obj.value("request").toObject();
        if (!request.empty()) {
            QString type = request.value("type").toString();
            if (type == "blocklist") {
                qInfo() << "Sending blocklist";
                sendBlocklist();
            } else {
                qWarning() << "Recieved unknown request for " << type;
            }
        }
    }
}

void ExtensionIntegration::setBlocklist(const QStringList& blocklist, const QString &name = "") {
    m_blocklist = blocklist;
    m_blocklistName = name;
    sendBlocklist();
}
bool ExtensionIntegration::sendBlocklist() {
    QJsonDocument obj {
        QJsonObject{
            {"response", QJsonObject{
                {"type", "blocklist"},
                {"name", m_blocklistName},
                {"data", QJsonArray::fromStringList(m_blocklist)}
            }}
        }
    };
    return sendRaw(obj.toJson(QJsonDocument::Compact));
}

bool ExtensionIntegration::sendPing() {
    return sendRaw("{\"type\":\"ping\"}");
}

bool ExtensionIntegration::sendRaw(const QByteArray& bytes) {
    bool success{false};
    uint32_t header = bytes.size();
    for (QLocalSocket* client: std::as_const(m_clients)) {
        success |= (client->write(reinterpret_cast<char*>(&header), sizeof(header)) == sizeof(header));
        success |= (client->write(bytes) == bytes.length());
    }
    if (!success) qInfo() << "did not write to any clients";
    return success;
}

bool ExtensionIntegration::checkFirefoxEnabled() {
    m_firefoxEnabled = QFile::exists(m_firefoxNMManifestDir + "/Focacciat.json");
    emit firefoxEnabledChanged();
    return m_firefoxEnabled;
}
bool ExtensionIntegration::firefoxEnabled() const {
    return m_firefoxEnabled;
}

bool ExtensionIntegration::setFirefoxEnabled(bool b) {
    qInfo() << "toggling" << b;
    if (b) {
        if (!QDir(m_firefoxNMManifestDir).exists()) {
            qWarning() << "ExtensionIntegration: Firefox might not exist";
            return false;
        }
        QFile f = QFile(":/qt/qml/Focacciat/extensionmanifest.json");
        if (!f.exists()) {
            qWarning() << "ExtensionIntegration: Internal error HELPTHISPROGRAMISNTWORKINGWHEREISTHERESOURCE";
            return false;
        }
        qInfo() << "other file exists";
        if (!f.copy(m_firefoxNMManifestDir + "/Focacciat.json")) {
            qWarning() << "ExtensionIntegration: Error: Manifest copy failed";
        }
        m_firefoxEnabled = true;
        emit firefoxEnabledChanged();
    } else {
        QFile f = QFile(m_firefoxNMManifestDir + "/Focacciat.json");
        if (!f.exists()) {
            qWarning() << "ExtensionIntegration: It's already disabled.";
            return false;
        }
        if (!f.remove()) {
            qWarning() << "ExtensionIntegration: Error: Could not remove file.";
            return false;
        }
        m_firefoxEnabled = false;
        emit firefoxEnabledChanged();
    }
    return true;
}
