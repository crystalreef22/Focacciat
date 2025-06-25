#ifndef TODOMODEL_H
#define TODOMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>

#include <QTimer>

#include "todoitem.h"

class TodoModel : public QAbstractListModel
{
    Q_OBJECT
    //Q_PROPERTY(QVector *list READ list WRITE setList)

public:
    explicit TodoModel(QObject *parent = nullptr);

    enum Roles {
        ItemRole = Qt::UserRole,
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


public slots:
    void appendItem();
    void removeCompletedItems();




private:
    QVector<TodoItem *> _list;
    QModelIndex _activeIndex;
    QTimer _timer;
};

#endif // TODOMODEL_H
