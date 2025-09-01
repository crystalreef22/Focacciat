#include "globalstate.h"

GlobalState::GlobalState()
{
    m_todoModel = new TodoModel(this);
    m_projectModel = new ProjectModel(this);
    m_blocklistListModel = new BlocklistListModel(this);
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
BlocklistListModel* GlobalState::blocklistListModel() { return m_blocklistListModel; }

