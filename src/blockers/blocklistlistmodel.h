#ifndef BLOCKLISTLISTMODEL_H
#define BLOCKLISTLISTMODEL_H

#include <QAbstractListModel>
#include <QtQmlIntegration/qqmlintegration.h>
#include <QPointer>
#include "blocklist.h"

class BlocklistListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Blocklist *activeItem READ activeItem NOTIFY activeItemChanged FINAL)
    QML_ELEMENT
    QML_UNCREATABLE("Managed by GlobalState.h")

public:
    explicit BlocklistListModel(QObject *parent = nullptr);
    BlocklistListModel (const BlocklistListModel&) = delete;
    BlocklistListModel& operator=(const BlocklistListModel&) = delete;

    enum Roles {
        NameRole = Qt::UserRole,
        ItemRole,
        ActiveRole
    };
    Q_ENUM(Roles);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;


    Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    Blocklist* activeItem() const;

    QJsonObject serialize() const;
    void deserialize(const QJsonObject& json);

signals:
    void activeItemChanged();

public slots:
    void appendItem();

    bool removeItem(const QModelIndex &index);
    bool removeItem(int i);

private:
    QVector<Blocklist*> m_blocklists;
    QPersistentModelIndex m_activeIndex;
};

#endif // BLOCKLISTLISTMODEL_H
