#pragma once

#include "qqmlintegration.h"
#include "networkmanager.h"
#include "databasemanager.h"
#include <QObject>
#include "resources.h"
#include "settings.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QJsonArray>
#include "anime.h"

//TODO: make "service"/"website" ABC?
class Anilist : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit Anilist(QObject *parent = nullptr);
    void configureOAuth2();
    void writeAnimeToDatabase(const Anime &entry); //TODO: should be media eventually?

signals:
    void requestFinished(const QJsonObject& data);
    void viewerListsReady(const QList<Anime>& mediaList);
    void databaseReady();

public slots:
    void searchAnime();
    void getViewerId();
    //void getViewerList(); //should not be void eventually
    void getViewerLists();
    void getViewerName();
    QString getAnimeImage(int id);
    void populateDatabase();

    bool onPopulateDatabaseReady(const QList<Anime>& mediaList);
    bool onUpdateDatabaseReady(const QList<Anime>& mediaList);
    QVariantList readEntriesFromDB();
    QVariantList readAnimeWithEntriesFromDB(Anime::EntryStatus status = Anime::EntryStatus::INVALID);

    void updateDatabase();
    //bool insertAnime();

private:
    // Methods
    void connectSignals();
    void initializeAccountInfo();


    NetworkManager::PostRequest constructSearch(QString queryText, bool authorized = false, QJsonObject variables = {} );
    void sendAnilistRequest(const QString& queryText, const bool isAuthRequest, const QJsonObject& variables = QJsonObject(), std::function<void(const QJsonObject&)> callback = [](const QJsonObject&){});
    void sendAnilistRequest(const QString& queryText, const bool isAuthRequest, std::function<void(const QJsonObject&)> callback);
    void getSearchData(QNetworkReply* reply, std::function<void(const QJsonObject&)> callback);

    QVariantList convertToVariantList(const QList<Anime> &mediaList);

    void addListsToDB(const QList<Anime>& mediaList);
    void processViewerLists(const QJsonObject &data);

    void compareEntries(const QList<Anime>& mediaList);

    QStringList createDBTables();
    bool executeSQLScripts();
    void openDatabaseConnection();

    // Attributes
    NetworkManager& m_netRequest = NetworkManager::instance();
    DatabaseManager& m_dbManager = DatabaseManager::instance();

    QUrl m_anilistUrl = QUrl("https://graphql.anilist.co");
    QUrl m_aniListAuthUrl = QUrl("https://anilist.co/api/v2/oauth/authorize");

    Settings& m_settings = Settings::instance();
    Resources m_resources;
};
