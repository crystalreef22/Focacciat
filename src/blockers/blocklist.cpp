#include "blocklist.h"
#include "extensionintegration.h"

Blocklist::Blocklist(QObject *parent)
    : QObject{parent}
    , m_websiteList{}
{}

Blocklist::Blocklist(const QString &name, QObject *parent)
    : QObject{parent}
    , m_name{name}
{}

QString Blocklist::name() const { return m_name; }

void Blocklist::setName(const QString& value) {
    m_name = value;
    emit nameChanged();
}

bool Blocklist::applyBlocks() const{
    QStringList list = m_websiteList.split('\n');
    list.removeAll(QString(""));
    return ExtensionIntegration::instance()->sendBlocklist(list);
}

bool Blocklist::removeAllBlocks() {
    return ExtensionIntegration::instance()->sendBlocklist(QStringList());
}
