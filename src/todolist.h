#ifndef TODOLIST_H
#define TODOLIST_H

#include <QObject>
#include <QVector>

struct TodoItem {
    bool done;
    QString description;
    long long timeEstimate;
    long long timeElapsed;
    bool active;
};

class TodoList : public QObject
{
    Q_OBJECT
public:
    explicit TodoList(QObject *parent = nullptr);

    QVector<TodoItem> items() const;

    bool setItemAt(int index, const TodoItem &item);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
    void removeCompletedItems();

private:
    QVector<TodoItem> _items;
};

#endif // TODOLIST_H
