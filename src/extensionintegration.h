#ifndef EXTENSIONINTEGRATION_H
#define EXTENSIONINTEGRATION_H

#include <QObject>

class ExtensionIntegration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool firefoxEnabled READ firefoxEnabled WRITE setFirefoxEnabled NOTIFY firefoxEnabledChanged FINAL)
public:
    explicit ExtensionIntegration(QObject *parent = nullptr);
    Q_INVOKABLE bool checkFirefoxEnabled();
    bool firefoxEnabled() const;

    bool setFirefoxEnabled(bool b);

signals:
    void firefoxEnabledChanged();
private:
    bool m_firefoxEnabled;
    QString m_firefoxNMManifestDir;
};

#endif // EXTENSIONINTEGRATION_H
