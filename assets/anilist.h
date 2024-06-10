#pragma once

#include "qqmlintegration.h"
#include "networkmanager.h"
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

    NetworkManager& m_netRequest = NetworkManager::instance();

    QUrl m_anilistUrl = QUrl("https://graphql.anilist.co");
    QUrl m_aniListAuthUrl = QUrl("https://anilist.co/api/v2/oauth/authorize");

    Settings& m_settings = Settings::instance();
    Resources m_resources;

public:
    explicit Anilist(QObject *parent = nullptr);
    void configureOAuth2();
    void writeAnimeToDatabase(const Anime &entry); //TODO: should be media eventually?
    void populateDatabase(const QList<Anime> &mediaList);

signals:
    void requestFinished(const QJsonObject& data);
    void viewerListsReady(const QList<Anime>& mediaList);

public slots:
    void searchAnime();
    void getViewerId();
    //void getViewerList(); //should not be void eventually
    void getViewerLists();
    void getViewerName();

    bool onViewerListsReady(const QList<Anime>& mediaList);
    //bool insertAnime();

private:
    void connectSignals();
    void initializeAccountInfo();

    NetworkManager::PostRequest constructSearch(QString queryText, bool authorized = false, QJsonObject variables = {} );
    void sendAnilistRequest(const QString& queryText, const bool isAuthRequest, const QJsonObject& variables = QJsonObject(), std::function<void(const QJsonObject&)> callback = [](const QJsonObject&){});
    void sendAnilistRequest(const QString& queryText, const bool isAuthRequest, std::function<void(const QJsonObject&)> callback);
    void getSearchData(QNetworkReply* reply, std::function<void(const QJsonObject&)> callback);
    QStringList createDBTables();
    void openDatabaseConnection();
};
