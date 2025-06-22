#ifndef TODOITEM_H
#define TODOITEM_H

#include <QObject>

class TodoItem : public QObject
{
    Q_OBJECT
public:
    explicit TodoItem(QObject *parent = nullptr);

signals:
};

#endif // TODOITEM_H
