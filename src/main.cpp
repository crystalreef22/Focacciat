#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include "todolist.h"
#include "focustimer.h"
#include "todomodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<FocusTimer>("FocusAssist9", 1, 0, "FocusTimer");
    qmlRegisterType<TodoModel>("Todo", 1, 0, "TodoModel");
    qmlRegisterUncreatableType<TodoList>("Todo", 1, 0, "TodoList",
        QStringLiteral("TodoList should not be created in QML"));

    TodoList todoList;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("todoList"), &todoList);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("FocusAssist9", "Main");

    return app.exec();
}
