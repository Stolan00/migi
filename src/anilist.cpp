#include "assets/anilist.h"
#include "databasemanager.h"
#include "filewriter.h"
#include <QResource>

#include <QDebug>
// --------------------------------------------------------------------------------------------------------------------------
Anilist::Anilist(QObject *parent) : QObject(parent){
    connectSignals();
    initializeAccountInfo();
    m_settings.setValue(AppSettingsKey::DatabasePath, "database"); //why is this here
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::searchAnime() {
    //connect(&m_netRequest, &NetworkManager::responseReceived, this, &Anilist::responseReceived);

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

    //QNetworkReply* reply = m_netRequest.sendPostRequest(postData, m_anilistUrl, headers);
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
    Resources resources;
    QString queryText = resources.readResource(AppResourceKey::ALQueryViewerId).toString();

    bool isAuthRequest = true;

    auto callback = [this](const QJsonObject& data) {
        QJsonObject viewer = data["Viewer"].toObject();
        int viewerId = viewer["id"].toInt();

        qDebug() << "VIEWER ID: " << viewerId;

        m_settings.setValue(AppSettingsKey::AccountAnilistViewerId, viewerId);

        if (!m_settings.value(AppSettingsKey::AccountAnilistViewerId).isNull()) {
            qDebug() << "Id Written";
        } else {
            qDebug() << "Failed to write token";
        }
    };

    sendAnilistRequest(queryText, isAuthRequest, callback);
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::getViewerLists() {
    Resources resources;
    QString queryText = resources.readResource(AppResourceKey::ALQueryMLCollection).toString();

    QString viewerName = m_settings.value(AppSettingsKey::AccountAnilistViewerName).toString();

    QJsonObject variables {
        {"userName", viewerName}
    };

    bool isAuthRequest = true;

    // Define the callback function for processing the data
    auto callback = [this](const QJsonObject& data) {
        QList<Anime> animeList;

        QJsonObject mlc = data["MediaListCollection"].toObject();
        QJsonArray lists = mlc["lists"].toArray();

        for (const QJsonValue& listValue : lists) {
            if (!listValue.isObject()) continue;

            QJsonObject listObject = listValue.toObject();
            if (!listObject.contains("entries") || !listObject["entries"].isArray()) continue;

            QJsonArray entries = listObject["entries"].toArray();
            for (const QJsonValue& entryValue : entries) {
                QJsonObject jsonFromEntry = entryValue.toObject();
                QJsonObject mediaFromEntry = jsonFromEntry["media"].toObject();

                //qDebug() << mediaFromEntry;

                Anime animeFromEntry(mediaFromEntry);
                qDebug() << animeFromEntry.id << " TITLE: " << animeFromEntry.titleEnglish << " " << animeFromEntry.titleRomaji;

                animeList.append(animeFromEntry);
            }
        }
        // Emit the signal with the populated list
        qDebug() << "VIEWER LIST READY";
        emit viewerListsReady(animeList); //TODO: awkward because it emits a signal in a callback when the entire thing could just be a slot
    };

    sendAnilistRequest(queryText, isAuthRequest, variables, callback);
}
// --------------------------------------------------------------------------------------------------------------------------
bool Anilist::onViewerListsReady(const QList<Anime> &mediaList) {

    writeMediaListToDatabase(mediaList);

    return true;
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::getViewerName() {
    Resources resources;
    QString queryText = resources.readResource(AppResourceKey::ALQueryViewerName).toString();

    bool isAuthRequest = true;

    auto callback = [this](const QJsonObject& data) {
        QJsonObject viewer = data["Viewer"].toObject();
        QString viewerName = viewer["name"].toString();

        qDebug() << "VIEWER NAME: " << viewerName;

        m_settings.setValue(AppSettingsKey::AccountAnilistViewerName, viewerName);

        if (!m_settings.value(AppSettingsKey::AccountAnilistViewerName).isNull()) {
            qDebug() << "Name Written";
        } else {
            qDebug() << "Failed to write token";
        }
    };

    sendAnilistRequest(queryText, isAuthRequest, callback);
}
// --------------------------------------------------------------------------------------------------------------------------
// Helper function which makes variables optional
void Anilist::sendAnilistRequest(const QString& queryText, const bool isAuthRequest, std::function<void(const QJsonObject&)> callback) {
    sendAnilistRequest(queryText, isAuthRequest, QJsonObject(), callback);
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::sendAnilistRequest(const QString& queryText, const bool isAuthRequest, const QJsonObject& variables, std::function<void(const QJsonObject&)> callback) {
    NetworkManager::PostRequest postRequest = constructSearch(queryText, isAuthRequest, variables);
    QNetworkReply* reply = m_netRequest.sendPostRequest(postRequest);

    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        getSearchData(reply, callback);
    });
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::getSearchData(QNetworkReply* reply, std::function<void(const QJsonObject&)> callback) {
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (!statusCode.isValid()) {
        qDebug() << "Failed to get HTTP status code.";
        return;
    }

    int statusCodeInt = statusCode.toInt();
    qDebug() << "HTTP status code:" << statusCodeInt;

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

    QByteArray responseData = reply->readAll();
    //qDebug() << "Response:" << responseData;

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObject = responseDoc.object();
    QJsonObject data = responseObject["data"].toObject();

    callback(data);

    reply->deleteLater();
}
// --------------------------------------------------------------------------------------------------------------------------
NetworkManager::PostRequest Anilist::constructSearch(QString queryText, bool authorized, QJsonObject variables) {
    NetworkManager::PostRequest postRequest;
    Resources resources;

    QString mediaFields = resources.readResource(AppResourceKey::ALQueryMediaFields).toString();
    queryText.replace("{mediaFields}", mediaFields);

    QString mediaListFields = resources.readResource( AppResourceKey::ALQueryMLFields ).toString();
    queryText.replace("{mediaListFields}", mediaListFields);

    postRequest.postData["query"] = queryText;

    if (!variables.isEmpty()) {
        postRequest.postData["variables"] = variables;
    }

    QJsonObject headers {
        { "Content-Type", "application/json" },
        { "Accept", "application/json" },
    };

    if (authorized) {
        headers.insert("Authorization", QString("Bearer %1").arg
                                            (m_settings.value(AppSettingsKey::AccountAnilistToken).toString()));
    }

    postRequest.request = m_netRequest.createRequest(m_anilistUrl, headers);

    return postRequest;
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::writeMediaListToDatabase(const QList<Anime>& mediaList) {
    QString dbPath = m_settings.value(AppSettingsKey::DatabasePath).toString();
    DatabaseManager db(dbPath);

    db.deleteAllTables();

    createDBTables();

    if (!db.createConnection(dbPath)) {
        qDebug() << "Error: Unable to open database.";
        return;
    }

    QList<QHash<QString, QVariant>> valuesList;
    for (const Anime& anime : mediaList) {
        //qDebug() << anime.asHash();

        valuesList.append(anime.asHash());
    }

    if (!db.bulkInsertIntoTable("Anime", valuesList)) {
        qDebug() << "Bulk insert failed.";
    } else {
        qDebug() << "Bulk insert succeeded.";
    }

    db.printAllValuesFromTable("Anime");
}


// --------------------------------------------------------------------------------------------------------------------------
void Anilist::writeAnimeToDatabase(const Anime& entry) {
    QStringList tables = createDBTables();

    qDebug() << tables;

    QHash<QString, QVariant> mediaValues = entry.asHash();

    QString dbPath = m_settings.value(AppSettingsKey::DatabasePath).toString();
    qDebug() << dbPath;
    DatabaseManager db( dbPath ); //TODO: awkward because it creates a duplicate connection as the call to createDBTables()

    //db.insertIntoTable("Anime", mediaValues);
}
// --------------------------------------------------------------------------------------------------------------------------
QStringList Anilist::createDBTables() {
    QString dbPath = m_settings.value(AppSettingsKey::DatabasePath).toString();
    DatabaseManager db( dbPath );

    db.deleteAllTables();
    FileWriter files;
    QStringList tables = files.readFilesFromDirectory(":/assets/sql", "sql");

    for (auto& table : tables) {
        if(db.createTable(table))
            qDebug() << "TABLE CREATED";
    }

    return db.getAllTables();
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::openDatabaseConnection() {
    QString dbPath = m_settings.value(AppSettingsKey::DatabasePath).toString();
    DatabaseManager db( dbPath );
}

// --------------------------------------------------------------------------------------------------------------------------
void Anilist::connectSignals() {
    connect(this, &Anilist::viewerListsReady, this, &Anilist::onViewerListsReady); //TODO: not sure if this should go in the lambda or here (probably here)
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::initializeAccountInfo() {
    // if (m_settings.value(AppSettingsKey::AccountAnilistToken).isNull()) { //TODO: dont think isNull() is right for this, if a setting doesnt exist QSettings seems to return a QVariant string, "could not open file", need to fix later
    //     //FileWriter fileManager;
    //     //QString token = fileManager.readFile(QString("token.txt")); // This should prompt the user to authorize in the future

    //     //m_settings.setValue(AppSettingsKey::AccountAnilistToken, token);

    //     if ( !m_settings.value( AppSettingsKey::AccountAnilistToken ).isNull() )
    //         qDebug() << "Token Written";

    //     else qDebug() << "Failed to write token";

    // } else qDebug() << "Token already exists: ";

    // if (m_settings.value(AppSettingsKey::AccountAnilistViewerId).isNull()) { //TODO: dont think isNull() is right for this, if a setting doesnt exist QSettings seems to return a QVariant string, "could not open file", need to fix later

    //     qDebug() << "IS NULL";
    //     QJsonObject viewerId = getViewerId(); // This should prompt the user to authorize in the future

    //     qDebug() << "RIGHT HERE";
    //     qDebug() << viewerId["data"];
    //     //QString token = response
    //     m_settings.setValue(AppSettingsKey::AccountAnilistViewerId, viewerId);

    //     if ( !m_settings.value( AppSettingsKey::AccountAnilistViewerId ).isNull() )
    //         qDebug() << "Id Written";

    //     else qDebug() << "Failed to write token";

    // } else qDebug() << "Id already exists";
}
// --------------------------------------------------------------------------------------------------------------------------
