#ifndef EXTENSIONINTEGRATION_H
#define EXTENSIONINTEGRATION_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>
#include "extensionclient.h"
#include <QLocalServer>

class QQmlEngine;
class QJSEngine;

class ExtensionIntegration : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(bool firefoxEnabled READ firefoxEnabled WRITE setFirefoxEnabled NOTIFY firefoxEnabledChanged FINAL)
public:
    static ExtensionIntegration* instance();
    static ExtensionIntegration* create(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE bool checkFirefoxEnabled();
    bool firefoxEnabled() const;

    bool setFirefoxEnabled(bool b);

signals:
    void firefoxEnabledChanged();

private:
    explicit ExtensionIntegration(QObject *parent = nullptr);

    void connectNextSocket();
    void clientDisconnected();

    bool m_firefoxEnabled{false};
    QString m_firefoxNMManifestDir;
    QLocalServer m_server;
    QVector<ExtensionClient*> m_clients; // clients are automatically children of server, so are deleted
    static constexpr char m_serverName[] = "focacciat_nmhostpipe";

    size_t m_maxConnections{50};

    inline static ExtensionIntegration* m_pThis{nullptr}; // for singleton
};

#endif // EXTENSIONINTEGRATION_H
