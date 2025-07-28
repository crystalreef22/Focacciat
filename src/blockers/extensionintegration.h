#ifndef EXTENSIONINTEGRATION_H
#define EXTENSIONINTEGRATION_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>
#include <QLocalSocket>
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

public slots:
    bool sendPing();
    bool sendBlocklist(const QStringList& blocklist);

private:
    explicit ExtensionIntegration(QObject *parent = nullptr);

    void connectNextSocket();
    void readMessage(QLocalSocket* conn);
    void socketDisconnected();
    bool sendRaw(const QByteArray& bytes);

    bool m_firefoxEnabled;
    QString m_firefoxNMManifestDir;
    QLocalServer m_server;
    QVector<QLocalSocket*> m_clients; // clients are automatically children of server, so are deleted
    static constexpr char m_serverName[] = "focusassist9_nmhostpipe";

    inline static ExtensionIntegration* m_pThis;
};

#endif // EXTENSIONINTEGRATION_H
