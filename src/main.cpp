#include <QGuiApplication>
#include <QFontDatabase>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include "todomodel.h"
#include "components/timeinput.h"
#include "blocklist.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<TodoModel>("Todo", 1, 0, "TodoModel");
    qmlRegisterType<TimeInput>("components", 1, 0, "TimeInput");

    QQmlApplicationEngine engine;

    /*
    // set monospaced font
    const QFont monospacedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    engine.rootContext()->setContextProperty("monospacedFont", monospacedFont);
    */

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("FocusAssist9", "Main");

    QObject::connect(&app, &QGuiApplication::aboutToQuit, &app, []{Blocklist::removeAllBlocks();}); // remove all blocks
    return app.exec();
}
