#include "extensionintegration.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QLocalServer>
#include "extensionclient.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

ExtensionIntegration::ExtensionIntegration(QObject *parent)
    : QObject{parent}
{
    //return; // FIXME: extensionintegration
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
    if (m_clients.size() < m_maxConnections) {
        qWarning() << "max connections reached";
        return;
    }
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
    ExtensionClient* client = new ExtensionClient(conn, this);
    m_clients.append(client);
    connect(client, &ExtensionClient::disconnected, this, &ExtensionIntegration::clientDisconnected);
}

void ExtensionIntegration::clientDisconnected() {
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    if (socket) {
        m_clients.removeOne(socket);
        socket->deleteLater();
    }
}


void ExtensionIntegration::readMessage(QLocalSocket* conn) {
    // header is taken care of by extensionhost
    // TODO: write a proper schema
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
                qInfo() << "Sending blocklist to one";
                sendBlocklist(conn);
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
bool ExtensionIntegration::sendBlocklist(QLocalSocket* client) {
    QJsonDocument obj {
        QJsonObject{
            {"response", QJsonObject{
                {"type", "blocklist"},
                {"name", m_blocklistName},
                {"data", QJsonArray::fromStringList(m_blocklist)}
            }}
        }
    };
    return sendRaw(obj.toJson(QJsonDocument::Compact), client);
}

bool ExtensionIntegration::sendPing(QLocalSocket* client) {
    return sendRaw("{\"type\":\"ping\"}", client);
}

bool ExtensionIntegration::sendRaw(const QByteArray& bytes, QLocalSocket* client) {
    // TODO: this is a mess
    bool success{false};
    uint32_t header = bytes.size();
    if (client) {
        success |= (client->write(reinterpret_cast<char*>(&header), sizeof(header)) == sizeof(header));
        success |= (client->write(bytes) == bytes.length());
    } else {
        for (QLocalSocket* iclient: std::as_const(m_clients)) {
            success |= (iclient->write(reinterpret_cast<char*>(&header), sizeof(header)) == sizeof(header));
            success |= (iclient->write(bytes) == bytes.length());
        }
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
