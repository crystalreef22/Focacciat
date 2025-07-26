#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

class Blocklist : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString websiteList MEMBER m_websiteList NOTIFY websiteListChanged FINAL)
public:
    explicit Blocklist(QObject *parent = nullptr);

signals:
    void websiteListChanged();

public slots:

private:
    QString m_websiteList;
};

#endif // BLOCKLIST_H
