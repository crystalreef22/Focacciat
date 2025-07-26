#ifndef BLOCKLISTLISTMODEL_H
#define BLOCKLISTLISTMODEL_H

#include <QAbstractListModel>
#include <QtQmlIntegration/qqmlintegration.h>

class BlocklistListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit BlocklistListModel(QObject *parent = nullptr);

    enum Roles {
        DisplayRole = Qt::DisplayRole,
        ItemRole = Qt::UserRole
    };
    Q_ENUM(Roles);

    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    bool appendItem();

    bool removeItem(const QModelIndex &index);

private:
};

#endif // BLOCKLISTLISTMODEL_H
