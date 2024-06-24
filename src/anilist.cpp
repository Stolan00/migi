#include "anilist.h"
#include "filewriter.h"
#include <QResource>

#include <QDebug>
// --------------------------------------------------------------------------------------------------------------------------
Anilist::Anilist(QObject *parent) : QObject(parent){
    connectSignals();
    initializeAccountInfo();
    populateDatabase(); // For now I'm doing this instead of checking for updates manually
    qDebug() << "DONE";
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

    sendAnilistRequest(queryText, isAuthRequest, variables, [this](const QJsonObject& data) {
        this->processViewerLists(data);
    });
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

    sendAnilistRequest(queryText, isAuthRequest, callback);
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
    //addListsToDB(mediaList);

    disconnect(this, &Anilist::viewerListsReady, this, &Anilist::viewerListsReady);
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
void Anilist::updateListsInDB(const QList<Anime>& mediaList) {
    // QString entryQuery = m_resources.readResource(AppResourceKey::GetEntryRows).toString();

    // entryQuery.replace("--PLACEHOLDER--", QString("WHERE %1 = :value").arg("Entry.mediaId"));

    // QVariantList entryRows = m_dbManager.executeQuery(entryQuery);

    // for (const Anime& anime: mediaList) {
    //     QSqlQuery query;
    //     query.prepare(entryQuery);
    //     query.bindValue(":value", anime.id);

    //     m_dbManager.executeQuery(query);
    // }
}
// --------------------------------------------------------------------------------------------------------------------------
// TODO: use modifiedAt from Anilist for list entries to compare against local database and only update
//       as-needed
void Anilist::addListsToDB(const QList<Anime> &mediaList) {
    //m_dbManager.deleteAllTables();

    //createDBTables();
    //executeSQLScripts();

    QList<QHash<QString, QVariant>> mediaValuesList;
    QList<QHash<QString, QVariant>> myInfoValuesList;
    QList<QHash<QString, QVariant>> animeGenreList;
    QList<QHash<QString, QVariant>> studioList;
    QList<QHash<QString, QVariant>> animeStudioList;

    QSet<int> studioSet;
    QSet<QString> animeStudioSet;

    for (const Anime& anime : mediaList) {
        mediaValuesList.append(anime.asHash());
        myInfoValuesList.append(anime.myInfoAsHash());

        for (const QString& genre : anime.genres) {
            QHash<QString, QVariant> animeGenre {
                { "genreId", anime.getGenreIndex(genre) },
                { "animeId", anime.id }
            };
            animeGenreList.append(animeGenre);
        }

        for (const Anime::Studio& studio : anime.studios) {
            int studioKey = studio.studioId;
            QString animeStudioKey = QString("%1-%2").arg(anime.id).arg(studio.studioId);

            if (!studioSet.contains(studioKey)) {
                QHash<QString, QVariant> studioHash {
                    { "studioId", studio.studioId },
                    { "studioName", studio.studioName },
                    { "isMain", studio.isMain }
                };
                studioList.append(studioHash);
                studioSet.insert(studioKey);
            }

            if (!animeStudioSet.contains(animeStudioKey)) {
                QHash<QString, QVariant> animeStudio {
                    { "studioId", studio.studioId },
                    { "animeId", anime.id },
                    { "isMain", studio.isMain }
                };
                animeStudioList.append(animeStudio);
                animeStudioSet.insert(animeStudioKey);
            } else {
                qDebug() << "DUPLICATE FOUND" << animeStudioKey;
            }
        }
    }

    m_dbManager.executeSqlScript(toString(AppResourceKey::SQLPopulateTables));

    if ( !m_dbManager.bulkInsertIntoTable("Anime", mediaValuesList) ) {
        qDebug() << "Bulk insert failed.";
    }

    if ( !m_dbManager.bulkInsertIntoTable("Entry", myInfoValuesList) ) {
        qDebug() << "Bulk insert failed.";
    }

    if ( !m_dbManager.bulkInsertIntoTable("AnimeGenre", animeGenreList) ) {
        qDebug() << "Bulk insert failed.";
    }

    if ( !m_dbManager.bulkInsertIntoTable("Studio", studioList) ) {
        qDebug() << "Bulk insert failed.";
    }

    if ( !m_dbManager.bulkInsertIntoTable("AnimeStudio", animeStudioList) ) {
        qDebug() << "Bulk insert failed.";
    }
}
// --------------------------------------------------------------------------------------------------------------------------
QString Anilist::getAnimeImage(int id) {
    FileWriter files;
    QString imageName = QString("images/%1.png").arg(id);
    QString imagePath = QString(files.getAppDataPath(imageName));

    return imagePath;
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::writeAnimeToDatabase(const Anime& entry) {
    //QStringList tables = createDBTables();

    // qDebug() << tables;

    // QHash<QString, QVariant> mediaValues = entry.asHash();

    //db.insertIntoTable("Anime", mediaValues);
}
// --------------------------------------------------------------------------------------------------------------------------
bool Anilist::executeSQLScripts() {
    m_dbManager.executeSqlScript(toString(AppResourceKey::CreateModifiedTrigger));
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
