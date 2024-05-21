#include "assets/networkmanager.h"
#include "assets/oauthimplicit.h"
#include "assets/settings.h"
#include <QNetworkRequest>

// --------------------------------------------------------------------------------------------------------------------------
NetworkManager& NetworkManager::instance() {
    static NetworkManager instance;
    return instance;
}
// --------------------------------------------------------------------------------------------------------------------------
NetworkManager::NetworkManager() {
    qRegisterMetaType<QJsonObject>();
}
// --------------------------------------------------------------------------------------------------------------------------
NetworkManager::~NetworkManager() {

}
// --------------------------------------------------------------------------------------------------------------------------
void NetworkManager::printJsonObject(const QJsonObject& obj) {
    QJsonDocument doc(obj);
    QString strJson(doc.toJson(QJsonDocument::Indented));
    qDebug() << strJson;
}
// --------------------------------------------------------------------------------------------------------------------------
void NetworkManager::configureOAuth2(const QString& clientId, const QUrl& authUrl, const QUrl& accessUrl) {
    OAuth2ImplicitGrant oauth2;

    oauth2.configure(clientId, authUrl, accessUrl);
    oauth2.grant();
}
// --------------------------------------------------------------------------------------------------------------------------
QNetworkReply* NetworkManager::postRequest(const QByteArray& postData, const QUrl& url, const QJsonObject& headers) {
    Settings& settings = Settings::instance();

    QNetworkRequest request = createRequest(url, headers);

    QNetworkReply* reply = manager.post(request, postData);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Network request error:" << reply->errorString();
            reply->deleteLater();
            return;
        }
    });

    qDebug() << "RETURNING REPLY";

    return reply;
}
// --------------------------------------------------------------------------------------------------------------------------
// Helper function for constructing request with provided headers
QNetworkRequest NetworkManager::createRequest(const QUrl &url, const QJsonObject &headers) {
    QNetworkRequest request(url);

    // Loop through the headers JSON object and set headers
    for (auto it = headers.constBegin(); it != headers.constEnd(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toString().toUtf8());
    }

    return request;
}
// --------------------------------------------------------------------------------------------------------------------------
QJsonObject NetworkManager::replyToJson(QNetworkReply* reply) {
    QByteArray responseArray = reply->readAll();
    //qDebug() << responseArray;
    QJsonDocument responseJsonDoc = QJsonDocument::fromJson(responseArray);

    if (!responseJsonDoc.isObject()) {
        qDebug() << "Failed to parse JSON from response";
        reply->deleteLater();
        return QJsonObject();
    }

    QJsonObject responseData = responseJsonDoc.object();

    printJsonObject(responseData);


    emit responseReceived(responseData);
    reply->deleteLater();

    return responseData;
}
