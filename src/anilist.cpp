#include "assets/anilist.h"
#include "assets/filewriter.h"
#include <QResource>

#include <QDebug>
// --------------------------------------------------------------------------------------------------------------------------
Anilist::Anilist() : m_anilistUrl("https://graphql.anilist.co") {

    if (m_settings.value(AppSettingsKey::AccountAnilistToken).isNull()) { //TODO: dont think isNull() is right for this, if a setting doesnt exist QSettings seems to return a QVariant string, "could not open file", need to fix later
        FileWriter fileManager;
        QString token = fileManager.readFile(QString("token.txt")); // This should prompt the user to authorize in the future

        m_settings.setValue(AppSettingsKey::AccountAnilistToken, token);

        if ( !m_settings.value( AppSettingsKey::AccountAnilistToken ).isNull() )
            qDebug() << "Token Written";

        else qDebug() << "Failed to write token";

    } else qDebug() << "Token already exists";
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::searchAnime() {
    connect(&m_netRequest, &NetworkManager::responseReceived, this, &Anilist::responseReceived);

    QJsonObject variables;
    variables["id"] = "486"; //TODO: May want to add a constructSearch function later or something that returns json qbytearray

    QString queryText = readResourceFile(":/assets/gql/Media.gql"); //TODO: MAKE ENUM CLASS OR SOMETHING FOR THESE PATHS
    QString mediaFields = readResourceFile(":/assets/gql/MediaFields.gql");
    queryText.replace("{mediaFields}", mediaFields);

    QJsonObject query;
    query["query"] = queryText;

    //query["query"] = strings.animeInfo;
    query["variables"] = variables;

    QJsonDocument document(query);
    QByteArray postData = document.toJson();

    QJsonObject headers {
        {"Content-Type", "application/json"},
        {"Accept", "application/json"},
        {"Authorization", QJsonValue( m_settings.value( AppSettingsKey::AccountAnilistToken ).toString() )}
    };

    m_netRequest.postRequest(postData, m_anilistUrl, headers);
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::configureOAuth2() {
    QString clientID  = "18259",
            authUrl   = "https://anilist.co/api/v2/oauth/authorize?client_id=18259&response_type=token",
            accessUrl = "https://anilist.co/api/v2/oauth/pin";

    m_netRequest.configureOAuth2(clientID, authUrl, accessUrl);
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::getViewerId() {
    connect(&m_netRequest, &NetworkManager::responseReceived, this, &Anilist::responseReceived);

    QJsonObject variables;
    variables["id"] = "486"; //TODO: May want to add a constructSearch function later or something that returns json qbytearray

    QJsonObject query;

    QString queryText = readResourceFile(":/assets/gql/ViewerId.gql");

    query["query"] = queryText;

    query["variables"] = variables;

    QJsonDocument document(query);
    QByteArray postData = document.toJson();

    QJsonObject headers {
        {"Content-Type", "application/json"},
        {"Accept", "application/json"},
        {"Authorization", QJsonValue(m_settings.value(AppSettingsKey::AccountAnilistToken).toString())}
    };

    m_netRequest.postRequest(postData, m_anilistUrl, headers);
}
// --------------------------------------------------------------------------------------------------------------------------
QString Anilist::readResourceFile(const QString &resourcePath) { //TODO: definitely needs to go in another class
    QFile file(resourcePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open resource file:" << resourcePath;
        return QString();
    }

    QTextStream in(&file);
    QString content = in.readAll();
    content.replace('\n', ' ').replace("\r", ""); // Remove newline characters
    content = content.simplified();

    qDebug() << content;
    return content;
}
// --------------------------------------------------------------------------------------------------------------------------
