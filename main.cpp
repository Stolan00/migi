#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "assets/anilist.h"
#include "assets/filewriter.h"
#include "assets/settings.h"
#include "assets/animelistmodel.h"
#include "assets/databasemanager.h"
#include <QMetaType>
// --------------------------------------------------------------------------------------------------------------------------
static void connectToDatabase()
{
    QSqlDatabase database = QSqlDatabase::database();
    if (!database.isValid()) {
        database = QSqlDatabase::addDatabase("QSQLITE");
        if (!database.isValid())
            qFatal("Cannot add database: %s", qPrintable(database.lastError().text()));
    }

    // Define the subdirectory path within the writable AppData location
    const QString subDir = "/Data/db";
    QDir writeDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + subDir);

    // Create the subdirectory path if it doesn't exist
    if (!writeDir.exists() && !writeDir.mkpath(".")) {
        qFatal("Failed to create writable directory at %s", qPrintable(writeDir.absolutePath()));
    }

    // Ensure that we have a writable location on all devices
    const QString fileName = writeDir.absolutePath() + "/migi_database.sqlite3";
    // When using the SQLite driver, open() will create the SQLite database if it doesn't exist
    database.setDatabaseName(fileName);
    if (!database.open()) {
        qFatal("Cannot open database: %s", qPrintable(database.lastError().text()));
        QFile::remove(fileName);
    }

    FileWriter files;
    DatabaseManager& db = DatabaseManager::instance();
    QStringList tables = files.readFilesFromDirectory(":/assets/sql/tables", "sql");

    for (auto& table : tables) {
        if(db.createTable(table))
            qDebug() << "TABLE CREATE";
    }
}
// --------------------------------------------------------------------------------------------------------------------------
// Main entrypoint to program
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Migi");
    QCoreApplication::setApplicationName("Migi");

    connectToDatabase();

    qmlRegisterType<AnimeListModel>("com.migi.models", 1, 0, "AnimeListModel");

    qmlRegisterSingletonType<Settings>("AppSettings", 1, 0, "Settings", Settings::create);

    QQmlApplicationEngine engine;

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
// --------------------------------------------------------------------------------------------------------------------------

