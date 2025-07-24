#include "extensionintegration.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QLocalServer>
#include <QLocalSocket>

ExtensionIntegration::ExtensionIntegration(QObject *parent)
    : QObject{parent}
{
    m_firefoxNMManifestDir = QDir::homePath() + "/Library/Application Support/Mozilla/NativeMessagingHosts";

    qWarning() << "see https://doc.qt.io/qt-6/qlocalserver.html#removeServer";
    QLocalServer::removeServer(m_serverName);

    m_server.setSocketOptions(QLocalServer::UserAccessOption);
    m_server.listen(m_serverName);

    qInfo() << "listening on " << m_server.fullServerName();
    connect(&m_server, &QLocalServer::newConnection, this, &ExtensionIntegration::connectNextSocket);
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
    quint32 header;
    conn->read(reinterpret_cast<char*>(&header), sizeof(quint32));
    qInfo() << "header: " << header;
    QByteArray data{conn->readAll()};
    qInfo() << "recieved: " << data;
}

bool ExtensionIntegration::send(const QString& data) {
    bool success{false};
    QByteArray bytes = data.toUtf8();
    quint32 header = bytes.size();
    for (QLocalSocket* client: std::as_const(m_clients)) {
        success |= (client->write(reinterpret_cast<char*>(&header), sizeof(header)) == sizeof(header));
        success |= (client->write(bytes) == bytes.length());
    }
    if (!success) qInfo() << "did not write to any clients";
    return success;
}

bool ExtensionIntegration::checkFirefoxEnabled() {
    m_firefoxEnabled = QFile::exists(m_firefoxNMManifestDir + "/FocusAssist9.json");
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
        QFile f = QFile(":/qt/qml/FocusAssist9/extensionmanifest.json");
        if (!f.exists()) {
            qWarning() << "ExtensionIntegration: Internal error HELPTHISPROGRAMISNTWORKINGWHEREISTHERESOURCE";
            return false;
        }
        qInfo() << "other file exists";
        if (!f.copy(m_firefoxNMManifestDir + "/FocusAssist9.json")) {
            qWarning() << "ExtensionIntegration: Error: Manifest copy failed";
        }
        m_firefoxEnabled = true;
        emit firefoxEnabledChanged();
    } else {
        QFile f = QFile(m_firefoxNMManifestDir + "/FocusAssist9.json");
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
