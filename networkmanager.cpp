#include "networkmanager.h"
#include "oauthimplicit.h"
#include <QNetworkRequest>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    qRegisterMetaType<QJsonObject>();
}

void NetworkManager::sendRequest(const QByteArray &postData, const QUrl &url) {
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("User-Agent", "Qt Application");

    QNetworkReply *reply = manager.post(request, postData);
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

void NetworkManager::printJsonObject(const QJsonObject& obj) {
    QJsonDocument doc(obj);
    QString strJson(doc.toJson(QJsonDocument::Indented)); // Use QJsonDocument::Compact for non-formatted output
    qDebug() << strJson;
}

void NetworkManager::configureOAuth2(const QString& clientId, const QUrl& authUrl, const QUrl& accessUrl) {
    OAuth2ImplicitGrant oauth2;

    oauth2.grant();



    // QOAuth2AuthorizationCodeFlow oauth2;
    // oauth2.setClientIdentifier(clientId);
    // oauth2.setAuthorizationUrl(authUrl);
    // oauth2.setAccessTokenUrl(accessUrl);


    // connect(&oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    // oauth2.grant();
}
