#include "extensionintegration.h"
#include <QFile>
#include <QDir>
#include <QDebug>


ExtensionIntegration::ExtensionIntegration(QObject *parent)
    : QObject{parent}
{
    m_firefoxNMManifestDir = QDir::homePath() + "/Library/Application Support/Mozilla/NativeMessagingHosts";

    qInfo() << m_firefoxNMManifestDir;
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
