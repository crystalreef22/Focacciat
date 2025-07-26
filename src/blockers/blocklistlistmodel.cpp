#include "blocklistlistmodel.h"

BlocklistListModel::BlocklistListModel(QObject *parent)
    : QAbstractListModel(parent)
{}


int BlocklistListModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    return m_blocklists.length();
}

QVariant BlocklistListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Blocklist* const item = m_blocklists.at(index.row());
    switch (role) {
    case NameRole:
        return QVariant(item->name);
    case ItemRole:
        return QVariant::fromValue(item);
    }

    return QVariant();
}

bool BlocklistListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == NameRole && index.isValid() && data(index, role) != value) {
        m_blocklists.at(index.row())->name = value.toString();
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags BlocklistListModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> BlocklistListModel::roleNames() const {
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[ItemRole] = "item";
    return names;
}

