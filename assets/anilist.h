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
    void createDBTables();

signals:
    void responseReceived(const QJsonObject& response);

public slots:
    void searchAnime();
    QJsonObject getViewerId();
    //void getViewerList(); //should not be void eventually
    void getViewerLists();
    void getViewerName();
    //bool insertAnime();

private:
    void initializeAccountInfo();
};
