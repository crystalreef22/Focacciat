#include "globalstate.h"
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QtCore/qjsonobject.h>

GlobalState::GlobalState()
{
    m_todoModel = new TodoModel(this);
    m_projectModel = new ProjectModel(this);
    m_blocklistManager = new BlocklistManager(this);
    m_saveTimer.setTimerType(Qt::VeryCoarseTimer);
    m_saveTimer.setInterval(1000*60*3);
    connect(&m_saveTimer, &QTimer::timeout, this, &GlobalState::serializeToFile);
}

GlobalState* GlobalState::instance() {
    if (m_pThis == nullptr) // avoid creation of new instances
        m_pThis = new GlobalState;
    return GlobalState::m_pThis;
}
GlobalState* GlobalState::create(QQmlEngine *engine, QJSEngine *scriptEngine) {
    return GlobalState::instance();
}

TodoModel* GlobalState::todoModel() { return m_todoModel; }
ProjectModel* GlobalState::projectModel() { return m_projectModel; }
BlocklistManager* GlobalState::blocklistManager() { return m_blocklistManager; }

bool GlobalState::serializeToFile() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qInfo() << path;
    QDir().mkpath(path);

    QFile file{path+"/appdata.json"};

    QJsonDocument appdata(QJsonObject {
        {"version", 0},
        {"todoEntries", m_todoModel->serialize()},
        {"blocklists", m_blocklistManager->serialize()}
    });

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    bool success = file.write(appdata.toJson());
    return success;
}

bool GlobalState::deserializeFromFile() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QFile file{path+"/appdata.json"};

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    // FIXME: does not check if read properly
    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    file.close();

    int version = json["version"].toInt(-1);
    if (version != 0) {
        qWarning() << "Tried to load from different json version than expected";
        return false;
    }

    // IMPORTANT: initialize blocklists first as todoEntries requires blocklists to attach to
    const QJsonObject& blocklists = json["blocklists"].toObject();
    const QJsonObject& todoEntries = json["todoEntries"].toObject();

    m_blocklistManager->deserialize(blocklists);
    m_todoModel->deserialize(todoEntries);

    return true;
}

