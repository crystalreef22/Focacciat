#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractListModel>
#include <QtQmlIntegration/qqmlintegration.h>
#include "projectitem.h"

class QQmlEngine;
class QJSEngine;

class ProjectModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    static ProjectModel* instance();
    static ProjectModel* create(QQmlEngine *engine, QJSEngine *scriptEngine);

    enum Roles { ItemRole = Qt::UserRole };
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
    explicit ProjectModel(QObject *parent = nullptr);
    QVector<ProjectItem *> m_projectItems;

    inline static ProjectModel* m_pThis{nullptr};
};

#endif // PROJECTMODEL_H
