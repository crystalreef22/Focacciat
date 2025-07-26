#ifndef EXTENSIONINTEGRATION_H
#define EXTENSIONINTEGRATION_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>
#include <QLocalSocket>
#include <QLocalServer>

class ExtensionIntegration : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(bool firefoxEnabled READ firefoxEnabled WRITE setFirefoxEnabled NOTIFY firefoxEnabledChanged FINAL)
public:
    explicit ExtensionIntegration(QObject *parent = nullptr);
    Q_INVOKABLE bool checkFirefoxEnabled();
    bool firefoxEnabled() const;

    bool setFirefoxEnabled(bool b);

signals:
    void firefoxEnabledChanged();

public slots:
    bool send(const QString& data);

private:
    void connectNextSocket();
    void readMessage(QLocalSocket* conn);
    void socketDisconnected();

    bool m_firefoxEnabled;
    QString m_firefoxNMManifestDir;
    QLocalServer m_server;
    QVector<QLocalSocket*> m_clients;
    static constexpr char m_serverName[] = "focusassist9_nmhostpipe";
};

#endif // EXTENSIONINTEGRATION_H
