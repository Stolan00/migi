#include "assets/anilist.h"
#include "assets/filewriter.h"
#include "assets/resources.h"
#include <QResource>

#include <QDebug>
// --------------------------------------------------------------------------------------------------------------------------
Anilist::Anilist(QObject *parent) : QObject(parent){
    initializeAccountInfo();
    qRegisterMetaType<QJsonArray>();

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
    QJsonObject query;

    Resources resources;
    QString queryText = resources.readResource(AppResourceKey::ALQueryViewerId).toString();

    query["query"] = queryText;

    QJsonDocument document(query);
    QByteArray postData = document.toJson(QJsonDocument::Compact);

    QJsonObject headers {
        { "Content-Type", "application/json" },
        { "Accept", "application/json" },
        { "Authorization", QString("Bearer %1").arg(m_settings.value(AppSettingsKey::AccountAnilistToken).toString()) }
    };

    QNetworkReply* reply = m_netRequest.postRequest(postData, m_anilistUrl, headers);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        if (statusCode.isValid()) {
            int statusCodeInt = statusCode.toInt();
            qDebug() << "HTTP status code:" << statusCodeInt;
        } else {
            qDebug() << "Failed to get HTTP status code.";
        }

        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error:" << reply->errorString();
        } else {
            QByteArray responseData = reply->readAll();
            qDebug() << "Response:" << responseData;

            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = responseDoc.object();
            qDebug() << responseObject;

            QJsonObject data = responseObject["data"].toObject();
            qDebug() << "DATA: " << data;
            QJsonObject viewer = data["Viewer"].toObject();
            qDebug() << "Viewer: " << viewer;
            int viewerId = viewer["id"].toInt();

            qDebug() << "VIEWER ID: " << viewerId;

            m_settings.setValue(AppSettingsKey::AccountAnilistViewerId, viewerId);

            if ( !m_settings.value( AppSettingsKey::AccountAnilistViewerId ).isNull() )
                qDebug() << "Id Written";

            else qDebug() << "Failed to write token";
            // Process the JSON response as needed
        }

        reply->deleteLater();
    });

    // You cannot return the JSON object directly here as the request is asynchronous
    // Instead, handle the processing inside the lambda connected to the finished signal

    return QJsonObject(); // Return an empty JSON object or handle it as needed
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::getViewerLists() {
    QJsonObject query;
    Resources resources;

    QString viewerName = m_settings.value(AppSettingsKey::AccountAnilistViewerName).toString();

    QString queryText = resources.readResource(AppResourceKey::ALQueryMLCollection).toString();

    QString mediaFields = resources.readResource( AppResourceKey::ALQueryMediaFields ).toString();
    QString mediaListFields = resources.readResource( AppResourceKey::ALQueryMLFields ).toString();
    queryText.replace("{mediaFields}", mediaFields);
    queryText.replace("{mediaListFields}", mediaListFields);

    query["query"] = queryText;

    QJsonObject variables {
        {"userName", viewerName}
    };

    query["variables"] = variables;

    QJsonDocument document(query);
    QByteArray postData = document.toJson(QJsonDocument::Compact);

    qDebug() << postData;

    QJsonObject headers {
        { "Content-Type", "application/json" },
        { "Accept", "application/json" },
        { "Authorization", QString("Bearer %1").arg(m_settings.value(AppSettingsKey::AccountAnilistToken).toString()) }
    };

    QNetworkReply* reply = m_netRequest.postRequest(postData, m_anilistUrl, headers);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        if (statusCode.isValid()) {
            int statusCodeInt = statusCode.toInt();
            qDebug() << "HTTP status code:" << statusCodeInt;
        } else {
            qDebug() << "Failed to get HTTP status code.";
        }

        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error:" << reply->errorString();
        } else {
            QByteArray responseData = reply->readAll();
            //qDebug() << "Response:" << responseData;

            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = responseDoc.object();
            //qDebug() << responseObject;

            QJsonObject data    = responseObject["data"].toObject();
            QJsonObject mlc     = data["MediaListCollection"].toObject();
            QJsonArray lists   = mlc["lists"].toArray();
            //qDebug() << lists;

            writeToDatabase(lists);

            //QJsonObject entries = lists["entries"].toObject();

            //qDebug() << entries;

            // qDebug() << "Viewer: " << viewer;
            // int viewerId = viewer["id"].toInt();

            // qDebug() << "VIEWER ID: " << viewerId;

            //m_settings.setValue(AppSettingsKey::AccountAnilistViewerId, viewerId);

            //if ( !m_settings.value( AppSettingsKey::AccountAnilistViewerId ).isNull() )
                //qDebug() << "Id Written";

            //else qDebug() << "Failed to write token";
            // Process the JSON response as needed
        }

        reply->deleteLater();
    });
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::getViewerName() {
    QJsonObject query;

    Resources resources;
    QString queryText = resources.readResource(AppResourceKey::ALQueryViewerName).toString();

    query["query"] = queryText;

    QJsonDocument document(query);
    QByteArray postData = document.toJson(QJsonDocument::Compact);

    QJsonObject headers {
        { "Content-Type", "application/json" },
        { "Accept", "application/json" },
        { "Authorization", QString("Bearer %1").arg(m_settings.value(AppSettingsKey::AccountAnilistToken).toString()) }
    };

    QNetworkReply* reply = m_netRequest.postRequest(postData, m_anilistUrl, headers);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        if (statusCode.isValid()) {
            int statusCodeInt = statusCode.toInt();
            qDebug() << "HTTP status code:" << statusCodeInt;
        } else {
            qDebug() << "Failed to get HTTP status code.";
        }

        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error:" << reply->errorString();
        } else {
            QByteArray responseData = reply->readAll();
            qDebug() << "Response:" << responseData;

            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
            QJsonObject responseObject = responseDoc.object();
            qDebug() << responseObject;

            QJsonObject data = responseObject["data"].toObject();
            qDebug() << "DATA: " << data;
            QJsonObject viewer = data["Viewer"].toObject();
            qDebug() << "Viewer: " << viewer;
            QString viewerName = viewer["name"].toString();

            qDebug() << "VIEWER ID: " << viewerName;

            m_settings.setValue(AppSettingsKey::AccountAnilistViewerName, viewerName);

            if ( !m_settings.value( AppSettingsKey::AccountAnilistViewerName ).isNull() )
                qDebug() << "Id Written";

            else qDebug() << "Failed to write token";
            // Process the JSON response as needed
        }

        reply->deleteLater();
    });
}
// --------------------------------------------------------------------------------------------------------------------------
void Anilist::writeToDatabase(QJsonArray& lists) {
    QFile file("anime.xml");

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Cannot open file for writing: %s", qPrintable(file.errorString()));
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("database");

    for (const QJsonValue &listValue : lists) {
        QJsonObject listObject = listValue.toObject();
        QJsonArray entries = listObject["entries"].toArray();

        // Process each entry in the entries array
        for (const QJsonValue &entryValue : entries) {
            QJsonObject entryObject = entryValue.toObject();

            // Extract fields from entryObject as needed
            int id = entryObject["id"].toInt();
            QString status = entryObject["status"].toString();

            QJsonObject media = entryObject["media"].toObject();
            QString titleRomaji = media["title"].toObject()["romaji"].toString();
            QString titleEnglish = media["title"].toObject()["english"].toString();
            QString titleNative = media["title"].toObject()["native"].toString();


            // Start Anime write
            stream.writeStartElement("anime");
                // Start ID write
                stream.writeStartElement("id");
                    stream.writeAttribute("name", "anilist");
                    stream.writeCharacters(QString::number(id));
                stream.writeEndElement(); // id

                // Start Title Write
                stream.writeStartElement("title");
                    stream.writeStartElement("romaji");
                    stream.writeCDATA(titleRomaji);
                    stream.writeEndElement();

                    stream.writeStartElement("english");
                    stream.writeCDATA(titleEnglish);
                    stream.writeEndElement();

                    stream.writeStartElement("native");
                    stream.writeCDATA(titleNative);
                    stream.writeEndElement();
                stream.writeEndElement(); //title

            stream.writeEndElement(); // anime
        }
    }

    stream.writeEndElement(); //database

    stream.writeEndDocument();
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
