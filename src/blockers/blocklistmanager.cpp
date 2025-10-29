#include "blocklistmanager.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

class QUuid;

BlocklistManager::BlocklistManager(QObject *parent)
    : QAbstractListModel(parent)
{
    appendItem();
}

BlocklistManager::~BlocklistManager() {
    for (auto item : std::as_const(m_blocklists)) {
        delete item;
    }
}

int BlocklistManager::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_blocklists.length();
}

QVariant BlocklistManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Blocklist* const item = m_blocklists.at(index.row());
    switch (role) {
    case NameRole:
        return QVariant(item->name());
    case ItemRole:
        return QVariant::fromValue(item);
    case ActiveRole:
        return QVariant(index == m_activeIndex);
    case WebsiteListRole:
        return QVariant(item->websiteList());
    }

    return QVariant();
}

bool BlocklistManager::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) [[unlikely]] {
        return false;
    }
    Blocklist* item = m_blocklists.at(index.row());
    switch (role) {
    case NameRole:
        item->setName(value.toString());
        emit dataChanged(index, index, {NameRole});
        return true;
    case ActiveRole:
        if (value.toBool()) {
            if (m_activeIndex != index) { // one alternative may be data(index, role) != value at top
                const QModelIndex oldIndex = m_activeIndex;
                m_activeIndex = index;
                emit activeItemChanged();
                if (oldIndex.isValid()) {
                    emit dataChanged(oldIndex, oldIndex, {ActiveRole});
                }
                emit dataChanged(index, index, {ActiveRole});
                emit activeBlocklistModified();
            }
        } else {
            if (m_activeIndex == index) {
                // deactivate
                m_activeIndex = QPersistentModelIndex{};
                emit activeItemChanged();
                emit dataChanged(index, index, {ActiveRole});
                emit activeBlocklistModified();
            }
        }
        return true;
    case WebsiteListRole:
        item->setWebsiteList(value.toString());
        emit dataChanged(index, index, {WebsiteListRole});
        if (m_activeIndex == index) {
            emit activeBlocklistModified();
        }
        return true;
    }
return false;
}

Qt::ItemFlags BlocklistManager::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> BlocklistManager::roleNames() const {
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[ItemRole] = "item";
    names[ActiveRole] = "active";
    names[WebsiteListRole] = "websiteList";
    return names;
}

const Blocklist* BlocklistManager::activeItem() const {
    if (m_activeIndex.isValid())
        return m_blocklists.at(m_activeIndex.row());
    else
        return nullptr;
}

const Blocklist* BlocklistManager::blocklistFromUUID(QUuid uuid) const {
    for (Blocklist* bl : std::as_const(m_blocklists)) {
        if (bl->UUID() == uuid) {
            return bl;
        }
    }
    return nullptr;
}

bool BlocklistManager::appendWebsitesToActiveItem(const QStringList &items) {
    if (m_activeIndex.isValid()) {
        m_blocklists.at(m_activeIndex.row())->appendWebsites(items);
        return true;
    }
    else {
        return false;
    }
}

QJsonObject BlocklistManager::serialize() const {
    QJsonArray items{};
    for (Blocklist* bl : std::as_const(m_blocklists)) {
        items.append(bl->serialize());
    }

    return QJsonObject{
        {"items", items}
    };
}

void BlocklistManager::deserialize(const QJsonObject &json) {
    const QJsonArray& items = json.value("items").toArray();
    beginResetModel();
    for (auto item : std::as_const(m_blocklists)) {
        delete item;
    }
    m_blocklists = {};
    m_blocklists.reserve(items.size());
    for (const QJsonValue& value : items) {
        m_blocklists.append(Blocklist::deserialize(value.toObject()));
    }
    endResetModel();
    emit activeBlocklistModified();// TODO: only emit when needed
    emit activeItemChanged(); // todo: serialize active item// TODO: only emit when needed
}



void BlocklistManager::appendItem() {
    const int index = m_blocklists.size();
    beginInsertRows(QModelIndex{}, index, index);
    m_blocklists.append(new Blocklist("Blocklist " + QString::number(m_blocklists.length()+1)));
    endInsertRows();
}
bool BlocklistManager::removeItem(const QModelIndex &index) {
    if (!index.isValid()) return false;
    removeItem(index.row());
    return true;
}
bool BlocklistManager::removeItem(int i) {
    if (i < 0 || i >= m_blocklists.length()) return false;
    if (m_blocklists.length() == 1) {
        beginResetModel();
        delete m_blocklists.at(0);
        m_blocklists[0] = new Blocklist("Blocklist 1");
        endResetModel();
        emit activeBlocklistModified(); // TODO: only emit when needed
        emit activeItemChanged();
        return true;
    }
    beginRemoveRows(QModelIndex{}, i, i);
    delete m_blocklists.at(i);
    m_blocklists.removeAt(i);
    emit activeBlocklistModified(); // TODO: only emit when needed
    emit activeItemChanged();
    endRemoveRows();
    return true;
}

