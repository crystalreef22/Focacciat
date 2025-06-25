#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include "todolist.h"
#include "todomodel.h"
#include "formatutils.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<TodoModel>("Todo", 1, 0, "TodoModel");
    //qmlRegisterUncreatableType<TodoList>("Todo", 1, 0, "TodoList",
    //    QStringLiteral("TodoList should not be created in QML"));

    //qmlRegisterUncreatableType<FormatUtils>("FormatUtils", 1, 0, "FormatUtils",
    //    QStringLiteral("FormatUtils should not be created"));

    //TodoList todoList;
    FormatUtils formatUtilsObj;

    QQmlApplicationEngine engine;
    // make todoList available to qml
    //engine.rootContext()->setContextProperty(QStringLiteral("todoList"), &todoList);
    // make formatutils too
    engine.rootContext()->setContextProperty(QStringLiteral("FormatUtils"), &formatUtilsObj);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("FocusAssist9", "Main");

    return app.exec();
}
