#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H

#include <QObject>
#include <QQmlEngine>
#include <todomodel.h>
#include <projectmodel.h>
#include <blockers/blocklistlistmodel.h>
#include <QFile>

class QQmlEngine;
class QJSEngine;

class GlobalState : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(TodoModel* todoModel READ todoModel CONSTANT)
    Q_PROPERTY(ProjectModel* projectModel READ projectModel CONSTANT)
    Q_PROPERTY(BlocklistListModel* blocklistListModel READ blocklistListModel CONSTANT)
public:
    static GlobalState* create(QQmlEngine *, QJSEngine *);
    static GlobalState* instance();
    TodoModel* todoModel();
    ProjectModel* projectModel();
    BlocklistListModel* blocklistListModel();
    Q_INVOKABLE bool serializeToFile();
    Q_INVOKABLE bool deserializeFromFile();
private:
    explicit GlobalState();
    TodoModel* m_todoModel;
    ProjectModel* m_projectModel;
    BlocklistListModel* m_blocklistListModel;
    inline static GlobalState* m_pThis{nullptr};
    QTimer m_saveTimer;
    // TODO: better autosave
};

#endif // GLOBALSTATE_H
