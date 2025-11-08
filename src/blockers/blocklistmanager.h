#ifndef BLOCKLISTMANAGER_H
#define BLOCKLISTMANAGER_H

#include <QAbstractListModel>
#include <QtQmlIntegration/qqmlintegration.h>
#include <QPointer>
#include "blocklist.h"

class QUuid;

class BlocklistManager : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QModelIndex activeIndex READ activeIndex WRITE setActiveIndex NOTIFY activeIndexChanged FINAL)
    QML_ELEMENT
    QML_UNCREATABLE("Managed by GlobalState.h")

public:
    explicit BlocklistManager(QObject *parent = nullptr);
    BlocklistManager (const BlocklistManager&) = delete;
    BlocklistManager& operator=(const BlocklistManager&) = delete;
    ~BlocklistManager();
    void connectToTodoModel();

    enum Roles {
        NameRole = Qt::UserRole,
        ItemRole,
        ActiveRole,
        WebsiteListRole,
        UuidRole,
        ModelIndexRole
    };
    Q_ENUM(Roles);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void setActiveIndex(QModelIndex index);


    Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    QModelIndex activeIndex() const;
    const Blocklist* blocklistFromUUID(QUuid uuid) const;
    QPersistentModelIndex persistentModelIndexFromUUID(QUuid uuid) const;
    bool appendWebsitesToActiveItem(const QStringList& items);

    QJsonObject serialize() const;
    void deserialize(const QJsonObject& json);

    const Blocklist *activeItem() const;
signals:
    void activeIndexChanged();
    void activeBlocklistModified();

public slots:
    void appendItem();

    bool removeItem(const QModelIndex &index);
    bool removeItem(int i);

private:
    QVector<Blocklist*> m_blocklists;
    QPersistentModelIndex m_activeIndex;
};

#endif // BLOCKLISTMANAGER_H
