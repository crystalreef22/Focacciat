#ifndef TODOLIST_H
#define TODOLIST_H

#include <QObject>
#include <QVector>
#include <QTimer>

struct TodoItem {
    bool done;
    QString description;
    long long timeEstimate;
    long long timeElapsed;
};

class TodoList : public QObject
{
    Q_OBJECT
public:
    explicit TodoList(QObject *parent = nullptr);

    const QVector<TodoItem>& getItems() const;

    bool setItemAt(qsizetype index, const TodoItem &item);

    const TodoItem& getActiveItem() const;
    bool activeItemExists() const;
    qsizetype getActiveIndex() const;
    void setActiveIndex(qsizetype index);

    void updateTimeElapsed();

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(qsizetype index);
    void postItemRemoved();

    void timeElapsedUpdated(qsizetype index);

public slots:
    void appendItem();
    void removeCompletedItems();

private:
    QVector<TodoItem> _items;
    qsizetype _activeIndex{-1};
    long long _lastStartEpoch;

    QTimer _timer;

    void removeItem(qsizetype index);
};

#endif // TODOLIST_H
