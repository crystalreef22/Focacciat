#ifndef TODOMODEL_H
#define TODOMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>

#include <QTimer>

#include "todoitem.h"

class TodoModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(TodoItem* activeItem READ activeItem NOTIFY activeItemChanged FINAL)

public:
    explicit TodoModel(QObject *parent = nullptr);

    // delete copy constuctor: TodoItems will be in a weird linked unsafe state
    TodoModel (const TodoModel&) = delete;
    TodoModel& operator= (const TodoModel&) = delete;

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

    TodoItem* activeItem() const;

signals:
    void activeItemChanged();

public slots:
    void appendItem();
    void removeCompletedItems();




private:
    QVector<TodoItem *> _list; // TodoItems are parented so no destructor needed
    QPersistentModelIndex _activeIndex;
    QTimer _timer;
};

#endif // TODOMODEL_H
