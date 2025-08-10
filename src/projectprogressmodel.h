#ifndef PROJECTPROGRESSMODEL_H
#define PROJECTPROGRESSMODEL_H

#include <QAbstractListModel>
#include <QDateTime>

struct ProgressItem {
    QDateTime timestamp;
    int progress;
};

class ProjectProgressModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ProjectProgressModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QVector<ProgressItem*> progress;
};

#endif // PROJECTPROGRESSMODEL_H
