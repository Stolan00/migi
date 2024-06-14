#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "assets/anilist.h"
#include "assets/filewriter.h"
#include "assets/settings.h"
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

    FileWriter fw;
    engine.rootContext()->setContextProperty("fileWriter", &fw);

    qmlRegisterType<Anime>("App.Anime", 1, 0, "Anime");

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

