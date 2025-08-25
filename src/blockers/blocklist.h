#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include <QObject>

class Blocklist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString websiteList READ websiteList WRITE setWebsiteList NOTIFY websiteListChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(bool m_watching READ watching WRITE setWatching NOTIFY watchingChanged FINAL)
public:
    explicit Blocklist(QObject *parent = nullptr);
    Blocklist(const QString &name, QObject *parent = nullptr);
    ~Blocklist(); // unset m_watching

    // Rule of 5
    Blocklist(const Blocklist& other) noexcept;
    Blocklist(Blocklist&& other) noexcept;
    Blocklist& operator=(const Blocklist& other) noexcept;
    Blocklist& operator=(Blocklist&& other) noexcept;

    const QString& name() const;
    bool watching() const;
    const QString& websiteList() const;
    void setName(const QString& value);
    void setWatching(bool value);
    void setWebsiteList(const QString& value);

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
    bool m_watching{false};
};

#endif // BLOCKLIST_H
