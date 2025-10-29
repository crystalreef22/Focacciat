#include "blocklist.h"
#include <QJsonObject>

Blocklist::Blocklist()
    :m_UUID{QUuid::createUuid()}
{}

Blocklist::Blocklist(const QString &name)
    : m_name{name},
    m_UUID{QUuid::createUuid()}
{}

Blocklist::Blocklist(const QUuid &uuid)
    : m_UUID{uuid}
{}

const QString& Blocklist::name() const { return m_name; }
const QString& Blocklist::websiteList() const { return m_websiteList; }

QUuid Blocklist::UUID() const {
    return m_UUID;
}

void Blocklist::setName(const QString& value) {
    m_name = value;
    emit nameChanged();
}
void Blocklist::setWebsiteList(const QString& websiteList) {
    m_websiteList = websiteList;
    emit websiteListChanged();
}

void Blocklist::appendWebsites(const QStringList& values) {
    for (const QString& value : values) {
        m_websiteList += "\n" + value;
    }
    emit websiteListChanged();
}

QJsonObject Blocklist::serialize() const {
    return QJsonObject{
        {"name", m_name},
        {"UUID", m_UUID.toString()},
        {"websiteList", m_websiteList}
    };
}

Blocklist *Blocklist::deserialize(const QJsonObject& json) {
    QUuid id{QUuid::fromString(json.find("UUID")->toString())};
    if (id.isNull()) id = QUuid::createUuid();
    Blocklist* bl = new Blocklist(id);
    bl->m_name = json.find("name")->toString("");
    bl->m_websiteList = json.find("websiteList")->toString("");
    return bl;
}
