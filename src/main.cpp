#define nogui
#ifndef nogui
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#else
#include <focustimer.h>
#include <QDebug>
#endif

int main(int argc, char *argv[])
{
#ifndef nogui
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
    FocusTimer ft;
    qDebug() << ft.toString();
#endif
}
