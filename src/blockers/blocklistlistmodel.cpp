#include "blocklistlistmodel.h"

BlocklistListModel::BlocklistListModel(QObject *parent)
    : QAbstractListModel(parent)
{}


QModelIndex BlocklistListModel::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return QModelIndex{};
}

int BlocklistListModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant BlocklistListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

bool BlocklistListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags BlocklistListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

