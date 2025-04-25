#define noguitest
#ifndef noguitest
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#else
#include <focustimer.h>
#include <QDebug>
#include <QCoreApplication>
#endif

int main(int argc, char *argv[])
{
#ifndef noguitest
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("FocusAssist9", "Main");

    return app.exec();
#else
    QCoreApplication app(argc, argv);
    FocusTimer ft{10000};
    ft.start(100);
    return app.exec();
#endif
}
