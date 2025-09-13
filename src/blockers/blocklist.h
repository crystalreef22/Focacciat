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
    Q_PROPERTY(bool watching READ watching WRITE setWatching NOTIFY watchingChanged FINAL)
    Q_PROPERTY(QUuid UUID READ UUID CONSTANT FINAL)
public:
    explicit Blocklist(QObject *parent = nullptr);
    explicit Blocklist(QUuid UUID, QObject *parent = nullptr);
    explicit Blocklist(const QString &name, QObject *parent = nullptr);
    ~Blocklist(); // unset m_watching

    // Rule of 5
    Blocklist(const Blocklist& other) noexcept;
    Blocklist(Blocklist&& other) noexcept;
    Blocklist& operator=(const Blocklist& other) noexcept;
    Blocklist& operator=(Blocklist&& other) noexcept;

    const QString& name() const;
    bool watching() const;
    const QString& websiteList() const;
    QUuid UUID() const;
    void setName(const QString& value);
    void setWatching(bool value);
    void setWebsiteList(const QString& value);

    QJsonObject serialize() const;
    static Blocklist* deserialize(const QJsonObject& json, QObject* parent);

    Q_INVOKABLE static Blocklist* fromUUID(QUuid uuid);

signals:
    void websiteListChanged();
    void nameChanged();
    void watchingChanged();

public slots:
    void applyBlocks();
    static void removeAllBlocks();

private:
    QString m_websiteList;
    QString m_name;
    QUuid m_UUID;
    bool m_watching{false};
    inline static QHash<QUuid, Blocklist*> m_blocklistUUIDMap;
};

#endif // BLOCKLIST_H
