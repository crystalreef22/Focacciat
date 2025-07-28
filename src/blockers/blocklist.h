#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

class Blocklist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString websiteList MEMBER m_websiteList NOTIFY websiteListChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
public:
    explicit Blocklist(QObject *parent = nullptr);
    Blocklist(const QString &name, QObject *parent = nullptr);

    QString name() const;
    void setName(const QString& value);

    Q_INVOKABLE bool applyBlocks() const;
    static bool removeAllBlocks();

signals:
    void websiteListChanged();
    void nameChanged();

public slots:

private:
    QString m_websiteList;
    QString m_name;
};

#endif // BLOCKLIST_H
