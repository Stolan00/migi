#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaType>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QUrl>
#include <QDesktopServices>
#include <QQmlEngine>

class NetworkManager : public QObject {
    Q_OBJECT
public:
    struct PostRequest {
        QJsonObject postData;
        QNetworkRequest request;
    };

    static NetworkManager& instance();
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    void printJsonObject(const QJsonObject& obj);
    void configureOAuth2(const QString& clientId, const QUrl& authUrl, const QUrl& accessUrl);

    static QObject* create(QQmlEngine *engine, QJSEngine *scriptEngine);




signals:
    void responseReceived(const QJsonObject&response);

public slots:
    QNetworkRequest createRequest(const QUrl &url , const QJsonObject &headers);
    QNetworkReply*  sendPostRequest(PostRequest);
    QJsonObject     replyToJson(QNetworkReply* reply);

private:
    NetworkManager();
    ~NetworkManager();
    QNetworkAccessManager manager; //used to be a pointer not sure why

};
