#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractListModel>
#include "projectitem.h"

class ProjectModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ProjectModel(QObject *parent = nullptr);

    enum Roles {
        ItemRole = Qt::UserRole
    };
    Q_ENUM(Roles)

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

public slots:
    void appendItem();
    void removeItem(int i);

private:
    QVector<ProjectItem*> m_projectItems;
};

#endif // PROJECTMODEL_H
