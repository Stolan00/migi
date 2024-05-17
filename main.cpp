#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "qsdata.h"
#include "anilist.h"
#include "filewriter.h"
#include "settings.h"
#include <QMetaType>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("MigiSoft");
    QCoreApplication::setApplicationName("Migi");

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<Settings>("AppSettings", 1, 0, "Settings", Settings::create);

    Anilist anilist;
    engine.rootContext()->setContextProperty("anilist", &anilist);

    QSData strings;
    engine.rootContext()->setContextProperty("strings", &strings);

    FileWriter fw;
    engine.rootContext()->setContextProperty("fileWriter", &fw);

    const QUrl url(QStringLiteral("qrc:/anilist_project/Main.qml"));\

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}

