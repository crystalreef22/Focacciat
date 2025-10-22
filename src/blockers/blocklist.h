#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include <QObject>
#include <QUuid>
#include <QHash>

class QJsonObject;
class Blocklist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString websiteList READ websiteList WRITE setWebsiteList NOTIFY websiteListChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QUuid UUID READ UUID CONSTANT FINAL)
public:
    explicit Blocklist(QObject *parent = nullptr);
    explicit Blocklist(QUuid UUID, QObject *parent = nullptr);
    explicit Blocklist(const QString &name, QObject *parent = nullptr);
    ~Blocklist(); // remove from uuid map

    // Rule of 5
    Blocklist(const Blocklist& other) noexcept;
    Blocklist(Blocklist&& other) noexcept;
    Blocklist& operator=(const Blocklist& other) noexcept;
    Blocklist& operator=(Blocklist&& other) noexcept;

    const QString& name() const;
    const QString& websiteList() const;
    QUuid UUID() const;
    void setName(const QString& value);
    void setWebsiteList(const QString& value);
    void appendWebsites(const QString& value);

    QJsonObject serialize() const;
    static Blocklist* deserialize(const QJsonObject& json, QObject* parent);

    Q_INVOKABLE static Blocklist* fromUUID(QUuid uuid);

signals:
    void websiteListChanged();
    void nameChanged();

private:
    QString m_websiteList;
    QString m_name;
    QUuid m_UUID;
    // NOTE: before adding new members, make sure to include in constructors
    inline static QHash<QUuid, Blocklist*> m_blocklistUUIDMap;
};

#endif // BLOCKLIST_H
