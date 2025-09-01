#include "blocklistlistmodel.h"
#include <QDebug>

BlocklistListModel::BlocklistListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    appendItem();
}
BlocklistListModel* BlocklistListModel::instance() {
    if (m_pThis == nullptr) // avoid creation of new instances
        m_pThis = new BlocklistListModel;
    return BlocklistListModel::m_pThis;
}
BlocklistListModel* BlocklistListModel::create(QQmlEngine *engine, QJSEngine *scriptEngine) {
    return BlocklistListModel::instance();
}


int BlocklistListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
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
        return QVariant(item->name());
    case ItemRole:
        return QVariant::fromValue(item);
    }

    return QVariant();
}

bool BlocklistListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == NameRole && index.isValid() && data(index, role) != value) {
        m_blocklists.at(index.row())->setName(value.toString());
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

void BlocklistListModel::appendItem() {
    const int index = m_blocklists.size();
    beginInsertRows(QModelIndex{}, index, index);
    m_blocklists.append(new Blocklist("Blocklist " + QString::number(m_blocklists.length()+1), this));
    endInsertRows();
}
bool BlocklistListModel::removeItem(const QModelIndex &index) {
    if (!index.isValid()) return false;
    removeItem(index.row());
    return true;
}
bool BlocklistListModel::removeItem(int i) {
    if (i < 0 || i >= m_blocklists.length()) return false;
    if (m_blocklists.length() == 1) {
        beginResetModel();
        m_blocklists.at(0)->deleteLater();
        m_blocklists[0] = new Blocklist("Blocklist 1", this);
        endResetModel();
        return true;
    }
    beginRemoveRows(QModelIndex{}, i, i);
    m_blocklists.at(i)->deleteLater();
    m_blocklists.removeAt(i);
    endRemoveRows();
    return true;
}

