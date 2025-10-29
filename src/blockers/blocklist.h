#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include <QUuid>
#include <QHash>

class QJsonObject;
class Blocklist
{
public:
    Blocklist();
    explicit Blocklist(const QUuid &UUID);
    explicit Blocklist(const QString &name);

    const QString& name() const;
    const QString& websiteList() const;
    QUuid UUID() const;
    void setName(const QString& value);
    void setWebsiteList(const QString& value);
    void appendWebsites(const QString& value);

    QJsonObject serialize() const;
    static Blocklist* deserialize(const QJsonObject& json);

signals:
    void websiteListChanged();
    void nameChanged();

private:
    QString m_websiteList;
    QString m_name;
    QUuid m_UUID;
};

#endif // BLOCKLIST_H
