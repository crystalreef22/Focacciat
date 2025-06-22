#ifndef TODOMODEL_H
#define TODOMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>

#include "todoitem.h"

class TodoList;

class TodoModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QVector *list READ list WRITE setList)

public:
    explicit TodoModel(QObject *parent = nullptr);

    enum Roles {
        DoneRole = Qt::UserRole,
        DescriptionRole,
        TimeEstimateRole,
        TimeRemainingRole,
        TimeElapsedRole,
        ActiveRole
    };
    Q_ENUM(Roles);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;






private:
    QVector<TodoItem *> _list;
};

#endif // TODOMODEL_H
