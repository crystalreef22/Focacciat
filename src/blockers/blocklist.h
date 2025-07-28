#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

class Blocklist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString websiteList MEMBER m_websiteList NOTIFY websiteListChanged FINAL)
public:
    explicit Blocklist(QObject *parent = nullptr);
    Blocklist(const QString &name, QObject *parent = nullptr);

    QString name; // holds the name of the blocklist, optional

signals:
    void websiteListChanged();

public slots:
    const bool applyBlocks();
    static const bool removeAllBlocks();

private:
    QString m_websiteList;
};

#endif // BLOCKLIST_H
