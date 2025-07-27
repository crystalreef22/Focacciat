#include "blocklist.h"
#include "extensionintegration.h"

Blocklist::Blocklist(QObject *parent)
    : QObject{parent}
    , m_websiteList{}
{}

Blocklist::Blocklist(const QString &name) {
    this->name = name;
}

const bool Blocklist::applyBlocks() {
    QStringList list = m_websiteList.split('\n');
    list.removeAll(QString(""));
    return ExtensionIntegration::instance()->sendBlocklist(list);
}
