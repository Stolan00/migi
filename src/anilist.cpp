#include "assets/anilist.h"
#include "assets/filewriter.h"
#include "assets/resources.h"
#include <QResource>

#include <QDebug>
// --------------------------------------------------------------------------------------------------------------------------
Anilist::Anilist(QObject *parent) : QObject(parent){
    initializeAccountInfo();
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::searchAnime() {
    connect(&m_netRequest, &NetworkManager::responseReceived, this, &Anilist::responseReceived);

    QJsonObject variables;
    variables["id"] = "486"; //TODO: May want to add a constructSearch function later or something that returns json qbytearray

    Resources resources;

    QString queryText   = resources.readResource( AppResourceKey::ALQueryMedia ).toString();
    QString mediaFields = resources.readResource( AppResourceKey::ALQueryMediaFields ).toString();

    queryText.replace("{mediaFields}", mediaFields);

    QJsonObject query;
    query["query"] = queryText;

    query["variables"] = variables;

    QJsonDocument document(query);
    QByteArray postData = document.toJson();

    QJsonObject headers {
        {"Content-Type", "application/json"},
        {"Accept", "application/json"},
        {"Authorization", m_settings.value( AppSettingsKey::AccountAnilistToken ).toJsonValue() }
    };

    QNetworkReply* reply = m_netRequest.postRequest(postData, m_anilistUrl, headers);
    //qDebug() << reply->readAll();
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::configureOAuth2() {
    QString clientID  = "18259",
            authUrl   = "https://anilist.co/api/v2/oauth/authorize?client_id=18259&response_type=token",
            accessUrl = "https://anilist.co/api/v2/oauth/pin";

    m_netRequest.configureOAuth2(clientID, authUrl, accessUrl);
}
// --------------------------------------------------------------------------------------------------------------------------
QJsonObject Anilist::getViewerId() {
    connect(&m_netRequest, &NetworkManager::responseReceived, this, &Anilist::responseReceived);

    QJsonObject query;

    Resources resources;
    QString queryText = resources.readResource(AppResourceKey::ALQueryViewerId).toString();
    //qDebug() << resources.readResource(AppResourceKey::ALQueryViewerId);

    query["query"] = queryText;

    QJsonDocument document(query);
    QByteArray postData = document.toJson(QJsonDocument::Compact);

    QJsonObject headers {
        { "Content-Type", "application/json" },
        { "Accept", "application/json" },
        { "Authorization", m_settings.value(AppSettingsKey::AccountAnilistToken).toString() }
    };

    QNetworkReply* reply = m_netRequest.postRequest( postData, m_anilistUrl, headers);

    //qDebug() << reply->readAll();
    qDebug() << "POST DATA:" << postData;

    return m_netRequest.replyToJson( reply );
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::getViewerList() {
    connect(&m_netRequest, &NetworkManager::responseReceived, this, &Anilist::responseReceived);

    QJsonObject variables;
    variables["id"] = "486"; //TODO: May want to add a constructSearch function later or something that returns json qbytearray

    Resources resources;

    QString queryText   = resources.readResource( AppResourceKey::ALQueryMedia ).toString();
    QString mediaFields = resources.readResource( AppResourceKey::ALQueryMediaFields ).toString();

    queryText.replace("{mediaFields}", mediaFields);

    QJsonObject query;
    query["query"] = queryText;

    query["variables"] = variables;

    QJsonDocument document(query);
    QByteArray postData = document.toJson();

    QJsonObject headers {
        {"Content-Type", "application/json"},
        {"Accept", "application/json"},
        {"Authorization", m_settings.value( AppSettingsKey::AccountAnilistToken ).toJsonValue() }
    };

    m_netRequest.postRequest(postData, m_anilistUrl, headers);
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::initializeAccountInfo() {
    if (m_settings.value(AppSettingsKey::AccountAnilistToken).isNull()) { //TODO: dont think isNull() is right for this, if a setting doesnt exist QSettings seems to return a QVariant string, "could not open file", need to fix later
        //FileWriter fileManager;
        //QString token = fileManager.readFile(QString("token.txt")); // This should prompt the user to authorize in the future

        //m_settings.setValue(AppSettingsKey::AccountAnilistToken, token);

        if ( !m_settings.value( AppSettingsKey::AccountAnilistToken ).isNull() )
            qDebug() << "Token Written";

        else qDebug() << "Failed to write token";

    } else qDebug() << "Token already exists: ";

    if (m_settings.value(AppSettingsKey::AccountAnilistViewerId).isNull()) { //TODO: dont think isNull() is right for this, if a setting doesnt exist QSettings seems to return a QVariant string, "could not open file", need to fix later

        qDebug() << "IS NULL";
        QJsonObject viewerId = getViewerId(); // This should prompt the user to authorize in the future

        qDebug() << "RIGHT HERE";
        qDebug() << viewerId["data"];
        //QString token = response
        m_settings.setValue(AppSettingsKey::AccountAnilistViewerId, viewerId);

        if ( !m_settings.value( AppSettingsKey::AccountAnilistViewerId ).isNull() )
            qDebug() << "Id Written";

        else qDebug() << "Failed to write token";

    } else qDebug() << "Id already exists";
}
