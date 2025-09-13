#include "blocklist.h"
#include "extensionintegration.h"
#include <QJsonObject>

Blocklist::Blocklist(QObject *parent)
    : QObject{parent}
    , m_websiteList{}
{
    m_UUID = QUuid::createUuid();
    m_blocklistUUIDMap.insert(m_UUID, this);
}

Blocklist::Blocklist(QUuid UUID, QObject *parent)
    : QObject{parent}
    , m_websiteList{}
{
    m_UUID = UUID;
    m_blocklistUUIDMap.insert(m_UUID, this);
}

Blocklist::Blocklist(const QString &name, QObject *parent)
    : QObject{parent}
    , m_name{name}
{
    m_UUID = QUuid::createUuid();
    m_blocklistUUIDMap.insert(m_UUID, this);
}

Blocklist::~Blocklist() {
    m_blocklistUUIDMap.remove(m_UUID);
    if (m_watching) {
        removeAllBlocks();
    }
}

Blocklist::Blocklist(const Blocklist& other) noexcept
    : m_watching{false}
    , m_name{other.m_name}
    , m_websiteList{other.m_websiteList}
{
    m_blocklistUUIDMap.insert(m_UUID, this);
    m_UUID = QUuid::createUuid();
}

Blocklist& Blocklist::operator=(const Blocklist& other) noexcept {
    if (this != &other) {
        m_watching = false;
        m_name = other.m_name;
        m_websiteList = other.m_websiteList;
        m_UUID = QUuid::createUuid();
        m_blocklistUUIDMap.insert(m_UUID, this);
    }
    return *this;
}
Blocklist::Blocklist(Blocklist&& other) noexcept
    : m_watching{std::move(other.m_watching)}
    , m_name{std::move(other.m_name)}
    , m_websiteList{std::move(other.m_websiteList)}
{
    m_blocklistUUIDMap.insert(m_UUID, this);
}
Blocklist& Blocklist::operator=(Blocklist&& other) noexcept {
    if (this != &other) {
        m_watching = other.watching();
        m_name = std::move(other.m_name);
        m_websiteList = std::move(other.m_websiteList);
        m_blocklistUUIDMap.insert(m_UUID, this);
    }
    return *this;
}

const QString& Blocklist::name() const { return m_name; }
bool Blocklist::watching() const { return m_watching; }
const QString& Blocklist::websiteList() const { return m_websiteList; }

QUuid Blocklist::UUID() const {
    return m_UUID;
}

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

QJsonObject Blocklist::serialize() const {
    return QJsonObject{
        {"name", m_name},
        {"UUID", m_UUID.toString()},
        {"websiteList", m_websiteList}
    };
}

Blocklist *Blocklist::deserialize(const QJsonObject& json, QObject* parent) {
    QUuid id{QUuid::fromString(json.find("UUID")->toString())};
    if (id.isNull()) id = QUuid::createUuid();
    Blocklist* bl = new Blocklist(QUuid::fromString(json.find("UUID")->toString()), parent);
    bl->m_name = json.find("name")->toString();
    bl->m_websiteList = json.find("websiteList")->toString();
    return bl;
}

Blocklist *Blocklist::fromUUID(QUuid uuid) {
    return m_blocklistUUIDMap.value(uuid, nullptr);
}

void Blocklist::applyBlocks() {
    QStringList list = m_websiteList.split('\n');
    list.removeAll(QString(""));
    ExtensionIntegration::instance()->setBlocklist(list, m_name);

}

void Blocklist::removeAllBlocks() {
    ExtensionIntegration::instance()->setBlocklist(QStringList(), "None");
}
