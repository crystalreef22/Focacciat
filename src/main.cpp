#include <QGuiApplication>
#include <QFontDatabase>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include "todomodel.h"
#include "formatutils.h"
#include "components/timeinput.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<TodoModel>("Todo", 1, 0, "TodoModel");
    qmlRegisterType<TimeInput>("components", 1, 0, "TimeInput");
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

    // set monospaced font
    const QFont monospacedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    engine.rootContext()->setContextProperty("monospacedFont", monospacedFont);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("FocusAssist9", "Main");

    return app.exec();
}
