#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QDateTime>
#include <QObject>

#include "projectprogressmodel.h"

class ProjectItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString description MEMBER m_description)
    Q_PROPERTY(QDateTime dueDate MEMBER m_dueDate)
    Q_PROPERTY(QDateTime startDate MEMBER m_startDate)
    //Q_PROPERTY(TodoTemplateModel todoTemplateModel MEMBER m_todoTemplateModel)
    // # days total, # days passed, # days progressed, switch between discrete and regular bar by clicking
public:
    explicit ProjectItem(QObject *parent = nullptr);

signals:

public slots:

private:
    QString m_description;
    QDateTime m_dueDate;
    QDateTime m_startDate;
    ProjectProgressModel dailyProjectProgress;
};

#endif // PROJECTITEM_H
