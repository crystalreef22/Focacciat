#include <QGuiApplication>
#include <QFontDatabase>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include "todomodel.h"
#include "blocklist.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<TodoModel>("Todo", 1, 0, "TodoModel");

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
    engine.loadFromModule("Focacciat", "Main");

    QObject::connect(&app, &QGuiApplication::aboutToQuit, &app, []{Blocklist::removeAllBlocks();}); // remove all blocks
    return app.exec();
}
