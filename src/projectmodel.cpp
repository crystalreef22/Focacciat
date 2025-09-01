#include "projectmodel.h"

ProjectModel::ProjectModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int ProjectModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_projectItems.length();
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == ItemRole) {
        return QVariant::fromValue(m_projectItems.at(index.row()));
    }

    return QVariant();
}

Qt::ItemFlags ProjectModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ProjectModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[ItemRole] = "item";
    return names;
}

void ProjectModel::appendItem()
{
    int row = m_projectItems.length();
    beginInsertRows(QModelIndex{}, row, row);
    m_projectItems.append(new ProjectItem(this));
    endInsertRows();
}

void ProjectModel::removeItem(int i)
{
    beginRemoveRows(QModelIndex{}, i, i);
    m_projectItems.at(i)->deleteLater();
    m_projectItems.removeAt(i);
    endRemoveRows();
}
