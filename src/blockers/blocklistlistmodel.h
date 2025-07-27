#ifndef BLOCKLISTLISTMODEL_H
#define BLOCKLISTLISTMODEL_H

#include <QAbstractListModel>
#include <QtQmlIntegration/qqmlintegration.h>
#include "blocklist.h"

class BlocklistListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit BlocklistListModel(QObject *parent = nullptr);

    enum Roles {
        NameRole = Qt::UserRole,
        ItemRole
    };
    Q_ENUM(Roles);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;


    Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

public slots:
    void appendItem();
    void appendItem(Blocklist* blocklist);

    bool removeItem(const QModelIndex &index);
    bool removeItem(int i);

private:
    QVector<Blocklist*> m_blocklists;
};

#endif // BLOCKLISTLISTMODEL_H
