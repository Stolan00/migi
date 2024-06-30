#include "anilist.h"
#include "filewriter.h"
#include "assets/animelistmodel.h"
#include <QResource>

#include <QDebug>
// --------------------------------------------------------------------------------------------------------------------------
Anilist::Anilist(QObject *parent) : QObject(parent) {
    connectSignals();
    initializeAccountInfo();
    //updateDatabase();
    populateDatabase(); // For now I'm doing this instead of checking for updates manually
    qDebug() << "DONE";

    connect(this, &Anilist::anilistDataReceived, this, &Anilist::handleAnilistData);
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::searchAnime() {
    // Resources resources;
    // QString queryText = resources.readResource(AppResourceKey::ALQueryMedia).toString();
    // QString mediaFields = resources.readResource(AppResourceKey::ALQueryMLFields).toString();
    // queryText.replace("{mediaFields}", mediaFields);

    // QJsonObject variables;
    // variables["id"] = id;

    // bool isAuthRequest = true;

    // sendAnilistRequest(queryText, isAuthRequest, variables, callback);
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::configureOAuth2() {
    QString clientID  = "18259",
        authUrl   = "https://anilist.co/api/v2/oauth/authorize?client_id=18259&response_type=token",
        accessUrl = "https://anilist.co/api/v2/oauth/pin";

    m_netRequest.configureOAuth2(clientID, authUrl, accessUrl);
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::updateDatabase() {
    connect(this, &Anilist::viewerListsReady, this, &Anilist::onUpdateDatabaseReady, Qt::UniqueConnection);

    getViewerLists();
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::populateDatabase() {
    connect(this, &Anilist::viewerListsReady, this, &Anilist::onPopulateDatabaseReady, Qt::UniqueConnection);

    getViewerLists();
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

    sendAnilistRequest(queryText, isAuthRequest, variables, RequestType::PopulateDatabase);  // Or RequestType::UpdateDatabase as needed
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::processViewerLists(const QJsonObject& data) {
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
            // QJsonObject mediaFromEntry = jsonFromEntry["media"].toObject();

            //qDebug() << mediaFromEntry;

            Anime animeFromEntry(jsonFromEntry);
            //qDebug() << animeFromEntry.id << " TITLE: " << animeFromEntry.titleEnglish << " " << animeFromEntry.titleRomaji;

            animeList.append(animeFromEntry);
        }
    }
    // Emit the signal with the populated list
    qDebug() << "VIEWER LIST READY";
    emit viewerListsReady(animeList);
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

    //sendAnilistRequest(queryText, isAuthRequest, callback);
}
// --------------------------------------------------------------------------------------------------------------------------
bool Anilist::onPopulateDatabaseReady(const QList<Anime> &mediaList) {
    addListsToDB(mediaList);

    disconnect(this, &Anilist::viewerListsReady, this, &Anilist::viewerListsReady);

    emit databaseReady();
    qDebug() << "DATABASE READY EMITTED";

    return true;
}
// --------------------------------------------------------------------------------------------------------------------------
bool Anilist::onUpdateDatabaseReady(const QList<Anime>& mediaList) {
    updateListsInDB(mediaList);

    disconnect(this, &Anilist::viewerListsReady, this, &Anilist::viewerListsReady);

    emit databaseReady();

    return true;
}
// --------------------------------------------------------------------------------------------------------------------------
QVariantList Anilist::readEntriesFromDB() {
    QString getEntriesQuery = m_resources.readResource(AppResourceKey::GetEntryRows).toString();

    //QVariantList rows = m_dbManager.executeQuery(getEntriesQuery);

    //return rows;
}
// --------------------------------------------------------------------------------------------------------------------------
QVariantList Anilist::readAnimeWithEntriesFromDB(Anime::EntryStatus status) {
    // QString getAnimeAndEntryDataQuery = m_resources.readResource(AppResourceKey::GetAnimeAndEntryData).toString();

    // if (status != Anime::EntryStatus::INVALID) {
    //     getAnimeAndEntryDataQuery.replace("-- ", "");
    //     getAnimeAndEntryDataQuery.replace("{status}", QString::number(static_cast<int>(status)));
    // }

    // QVariantList rows = m_dbManager.executeQuery(getAnimeAndEntryDataQuery);

    // // TODO: not sure if this should happen in another cpp method or in the QML file
    // // also need to show userPreferred rather than defaulting to titleEnglish
    // std::sort(rows.begin(), rows.end(), [](const QVariant &a, const QVariant &b) {
    //     QVariantMap mapA = a.toMap();
    //     QVariantMap mapB = b.toMap();

    //     QString titleA = mapA.value("titleEnglish").toString();
    //     if (titleA.isEmpty()) {
    //         titleA = mapA.value("titleRomaji").toString();
    //     }

    //     QString titleB = mapB.value("titleEnglish").toString();
    //     if (titleB.isEmpty()) {
    //         titleB = mapB.value("titleRomaji").toString();
    //     }

    //     return titleA < titleB;
    // });

    // return rows;
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::compareEntries(const QList<Anime>& mediaList) {
    //QVariantList entries = readAnimeEntriesFromDB();

    for(const auto& media : mediaList) {
        QString findEntryQuery = QString("SELECT anilistModified FROM Entry WHERE Entry.mediaId = %1").arg(media.id);
    }
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
        }

        else {
            qDebug() << "Failed to write token";
        }
    };

    //sendAnilistRequest(queryText, isAuthRequest, callback);
}
// --------------------------------------------------------------------------------------------------------------------------
// Helper function which makes variables optional
void Anilist::sendAnilistRequest(const QString& queryText, const bool isAuthRequest, RequestType requestType) {
    sendAnilistRequest(queryText, isAuthRequest, QJsonObject(), requestType);
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::sendAnilistRequest(const QString& queryText, bool isAuthRequest, const QJsonObject& variables, RequestType requestType) {
    NetworkManager::PostRequest postRequest = constructSearch(queryText, isAuthRequest, variables);
    QNetworkReply* reply = m_netRequest.sendPostRequest(postRequest);

    connect(reply, &QNetworkReply::finished, this, [this, reply, requestType]() {
        QJsonObject data = getSearchData(reply);
        emit anilistDataReceived(requestType, data);
        reply->deleteLater();
    });
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::handleAnilistData(RequestType requestType, const QJsonObject& data) {
    switch (requestType) {
    case RequestType::PopulateDatabase:
    {
        processViewerLists(data);
        break;
    }
    case RequestType::UpdateDatabase:
    {
        processViewerLists(data);
        break;
    }
    case RequestType::FetchAnimeImage:

        break;
        // Add more cases as needed
    }
}
// --------------------------------------------------------------------------------------------------------------------------
QJsonObject Anilist::getSearchData(QNetworkReply* reply) {
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (!statusCode.isValid()) {
        qDebug() << "Failed to get HTTP status code.";
        return QJsonObject();
    }

    int statusCodeInt = statusCode.toInt();
    qDebug() << "HTTP status code:" << statusCodeInt;

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error:" << reply->errorString();
        return QJsonObject();
    }

    QByteArray responseData = reply->readAll();
    //qDebug() << "Response:" << responseData;

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObject = responseDoc.object();
    QJsonObject data = responseObject["data"].toObject();

    reply->deleteLater();

    return data;
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
void Anilist::updateListsInDB(const QList<Anime>& mediaList) {
    m_db.updateDatabase(mediaList);
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::addListsToDB(const QList<Anime> &mediaList) {
    m_db.addListsToDB(mediaList);
}
// --------------------------------------------------------------------------------------------------------------------------
QString Anilist::getAnimeImage(int id) {
    FileWriter files;
    QString imageName = QString("images/%1.png").arg(id);
    QString imagePath = QString(files.getAppDataPath(imageName));

    if (!QFile::exists(imagePath)) {
        // Image file does not exist, initiate search
        fetchAnimeData(id, RequestType::FetchAnimeImage);
    }

    return imagePath;
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::fetchAnimeData(int id, RequestType requestType) {
    Resources resources;
    QString queryText = resources.readResource(AppResourceKey::ALQueryMedia).toString();
    QString mediaFields = resources.readResource(AppResourceKey::ALQueryMLFields).toString();
    queryText.replace("{mediaFields}", mediaFields);

    QJsonObject variables;
    variables["id"] = id;

    bool isAuthRequest = true;

    sendAnilistRequest(queryText, isAuthRequest, variables, requestType);
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::handleFetchAnimeImage(const QJsonObject& animeData) {
    Anime animeObj(animeData);

    qDebug() << "Received anime data for ID:" << animeObj.id;
    qDebug() << "Anime image link:" << animeObj.imageLink;

    QString imagePath = QString("images/%1.png").arg(animeObj.id);

    // downloadAndSaveImage(animeObj.imageLink, imagePath);
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
void Anilist::writeAnimeToDatabase(const Anime& entry) {
    //QStringList tables = createDBTables();

    // qDebug() << tables;

    // QHash<QString, QVariant> mediaValues = entry.asHash();

    //db.insertIntoTable("Anime", mediaValues);
}
// --------------------------------------------------------------------------------------------------------------------------
bool Anilist::executeSQLScripts() {
    //m_dbManager.executeSqlScript(toString(AppResourceKey::CreateModifiedTrigger));
}
// --------------------------------------------------------------------------------------------------------------------------
//TODO: singleton? member?
void Anilist::openDatabaseConnection() {
    // QString dbPath = m_settings.value(AppSettingsKey::DatabasePath).toString();
    // DatabaseManager db( dbPath );
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::connectSignals() {
    //connect(this, &Anilist::viewerListsReady, this, &Anilist::onViewerListsReady); //TODO: not sure if this should go in the lambda or here (probably here)
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

    getViewerName();
}
// --------------------------------------------------------------------------------------------------------------------------
