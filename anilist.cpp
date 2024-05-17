#include "anilist.h"
#include "filewriter.h"
#include "settings.h"
#include <QDebug>

Anilist::Anilist() : m_anilist_url("https://graphql.anilist.co") {
    Settings& settings = Settings::instance();

    if (settings.value(AppSettingsKey::AccountAnilistToken).isNull()) { //TODO: dont think isNull() is right for this, if a setting doesnt exist QSettings seems to return a QVariant string, "could not open file", need to fix later
        FileWriter fileManager;
        QString token = fileManager.readFile(QString("token.txt")); // This should prompt the user to authorize in the future

        settings.setValue(AppSettingsKey::AccountAnilistToken, token);

        if ( !settings.value( AppSettingsKey::AccountAnilistToken ).isNull() )
            qDebug() << "Token Written";

        else qDebug() << "Failed to write token";

    } else qDebug() << "Token already exists";
}

void Anilist::search_anime() {
    connect(&m_netRequest, &NetworkManager::responseReceived, this, &Anilist::responseReceived);

    QJsonObject variables;

    variables["id"] = "486"; //TODO: May want to add a constructSearch function later or something that returns json qbytearray

    QJsonObject query;
    query["query"] = strings.animeInfo;
    query["variables"] = variables;

    QJsonDocument document(query);
    QByteArray postData = document.toJson();

    m_netRequest.sendRequest(postData, m_anilist_url);
}

void Anilist::configureOAuth2() {
    QString clientID  = "18259",
            authUrl   = "https://anilist.co/api/v2/oauth/authorize?client_id=18259&response_type=token",
            accessUrl = "https://anilist.co/api/v2/oauth/pin";

    m_netRequest.configureOAuth2(clientID, authUrl, accessUrl);
}
