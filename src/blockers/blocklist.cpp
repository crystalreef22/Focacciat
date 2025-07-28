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

Blocklist::~Blocklist() {
    if (m_watching) {
        removeAllBlocks();
    }
}

Blocklist::Blocklist(const Blocklist& other) noexcept
    : m_watching{false}
    , m_name{other.m_name}
    , m_websiteList{other.m_websiteList}
{}

Blocklist& Blocklist::operator=(const Blocklist& other) noexcept {
    if (this != &other) {
        m_watching = false;
        m_name = other.m_name;
        m_websiteList = other.m_websiteList;
    }
    return *this;
}
Blocklist::Blocklist(Blocklist&& other) noexcept
    : m_watching{std::move(other.m_watching)}
    , m_name{std::move(other.m_name)}
    , m_websiteList{std::move(other.m_websiteList)}
{}
Blocklist& Blocklist::operator=(Blocklist&& other) noexcept {
    if (this != &other) {
        m_watching = other.watching();
        m_name = std::move(other.m_name);
        m_websiteList = std::move(other.m_websiteList);
    }
    return *this;
}

const QString& Blocklist::name() const { return m_name; }
bool Blocklist::watching() const { return m_watching; }
const QString& Blocklist::websiteList() const { return m_websiteList; }

void Blocklist::setName(const QString& value) {
    m_name = value;
    emit nameChanged();
}
void Blocklist::setWatching(bool value) {
    m_watching = value;
    emit watchingChanged();
}
void Blocklist::setWebsiteList(const QString& websiteList) {
    m_websiteList = websiteList;
    emit websiteListChanged();
    if (m_watching) {
        applyBlocks();
    }
}

void Blocklist::applyBlocks() {
    QStringList list = m_websiteList.split('\n');
    list.removeAll(QString(""));
    ExtensionIntegration::instance()->setBlocklist(list, m_name);

}

void Blocklist::removeAllBlocks() {
    ExtensionIntegration::instance()->setBlocklist(QStringList(), "None");
}
