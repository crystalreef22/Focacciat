#include "blocklistlistmodel.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

BlocklistListModel::BlocklistListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    appendItem();
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

QJsonObject BlocklistListModel::serialize() const {
    QJsonArray items{};
    for (Blocklist* bl : std::as_const(m_blocklists)) {
        items.append(bl->serialize());
    }

    return QJsonObject{
        {"items", items}
    };
}

void BlocklistListModel::deserialize(const QJsonObject &json) {
    const QJsonArray& items = json.value("items").toArray();
    beginResetModel();
    for (auto item : std::as_const(m_blocklists)) {
        item->deleteLater();
    }
    m_blocklists = {};
    m_blocklists.reserve(items.size());
    for (const QJsonValue& value : items) {
        m_blocklists.append(Blocklist::deserialize(value.toObject(), this));
    }
    endResetModel();

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

