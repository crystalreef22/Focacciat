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
    return ExtensionIntegration::instance()->sendBlocklist(m_websiteList.split('\n'));
}
