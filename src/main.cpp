#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include <QCommandLineParser>

#include "blocklist.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({"debugwindow", "Show debug window instead of main"}); // Boolean option
    parser.process(app);

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
    if (parser.isSet("debugwindow")) {
        engine.loadFromModule("Focacciat", "DebugView");
    } else {
        engine.loadFromModule("Focacciat", "Main");
    }

    QObject::connect(&app, &QGuiApplication::aboutToQuit, &app, [] {
        Blocklist::removeAllBlocks();
    }); // remove all blocks
    return app.exec();
}
