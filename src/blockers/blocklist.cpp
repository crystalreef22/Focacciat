#include "blocklist.h"
#include "extensionintegration.h"

Blocklist::Blocklist(QObject *parent)
    : QObject{parent}
    , m_websiteList{}
{}

Blocklist::Blocklist(const QString &name, QObject *parent)
    : QObject{parent}
    , name{name}
{}

const bool Blocklist::applyBlocks() {
    QStringList list = m_websiteList.split('\n');
    list.removeAll(QString(""));
    return ExtensionIntegration::instance()->sendBlocklist(list);
}

const bool Blocklist::removeAllBlocks() {
    return ExtensionIntegration::instance()->sendBlocklist(QStringList());
}
