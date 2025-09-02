#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include <QCommandLineParser>

#include "blocklist.h"
#include "globalstate.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({"debugwindow", "Show debug window instead of main"}); // Boolean option
    parser.process(app);

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    auto globalState = engine.singletonInstance<GlobalState *>("Focacciat", "GlobalState");
    globalState->deserializeFromFile();

    if (parser.isSet("debugwindow")) {
        engine.loadFromModule("Focacciat", "DebugView");
    } else {
        engine.loadFromModule("Focacciat", "Main");
    }

    QObject::connect(&app, &QGuiApplication::aboutToQuit, &app, [=] {
        Blocklist::removeAllBlocks();
        globalState->serializeToFile();
    }); // remove all blocks
    return app.exec();
}
