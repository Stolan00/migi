#include "assets/networkmanager.h"
#include "assets/oauthimplicit.h"
#include "assets/settings.h"
#include <QNetworkRequest>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    qRegisterMetaType<QJsonObject>();
}

void NetworkManager::printJsonObject(const QJsonObject& obj) {
    QJsonDocument doc(obj);
    QString strJson(doc.toJson(QJsonDocument::Indented)); // Use QJsonDocument::Compact for non-formatted output
    qDebug() << strJson;
}

void NetworkManager::configureOAuth2(const QString& clientId, const QUrl& authUrl, const QUrl& accessUrl) {
    OAuth2ImplicitGrant oauth2;

    oauth2.grant();
}

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

        QByteArray responseArray = reply->readAll();
        QJsonDocument responseJsonDoc = QJsonDocument::fromJson(responseArray);
        if (!responseJsonDoc.isObject()) {
            qDebug() << "Failed to parse JSON from response";
            reply->deleteLater();
            return;
        }

        QJsonObject responseData = responseJsonDoc.object();
        printJsonObject(responseData);
        emit responseReceived(responseData);
        reply->deleteLater();
    });

}

// Helper function for constructing request with provided headers
QNetworkRequest NetworkManager::createRequest(const QUrl &url, const QJsonObject &headers) {
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Set the Authorization header if the access token is provided

    // Loop through the headers JSON object and set headers
    for (auto it = headers.constBegin(); it != headers.constEnd(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toString().toUtf8());
    }

    return request;
}
