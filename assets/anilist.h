#pragma once

#include "qqmlintegration.h"
#include "networkmanager.h"
#include <QObject>
#include "resources.h"
#include "settings.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QJsonArray>


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
    void writeToDatabase(QJsonArray& entries);
    QStringList createDBTables();

signals:
    void requestFinished(const QJsonObject& data);
    //void responseReceived(const QJsonObject& response);

public slots:
    void searchAnime();
    QJsonObject getViewerId();
    //void getViewerList(); //should not be void eventually
    void getViewerLists();
    void getViewerName();
    //bool insertAnime();

private:
    void initializeAccountInfo();
    NetworkManager::PostRequest constructSearch(QString queryText, bool authorized = false, QJsonObject variables = {} );

    void sendAnilistRequest(const QString& queryText, const bool isAuthRequest, const QJsonObject& variables, std::function<void(const QJsonObject&)> callback);

};
