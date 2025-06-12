#ifndef TODOLIST_H
#define TODOLIST_H

#include <QObject>

class TodoList : public QObject
{
    Q_OBJECT
public:
    explicit TodoList(QObject *parent = nullptr);

signals:
};

#endif // TODOLIST_H
