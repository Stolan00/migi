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

class NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    void printJsonObject(const QJsonObject& obj);
    void configureOAuth2(const QString& clientId, const QUrl& authUrl, const QUrl& accessUrl);

signals:
    void responseReceived(const QJsonObject&response);

public slots:
    void sendRequest(const QByteArray& postData, const QUrl& url); //i dont think this should be void but it works currently because its a slot
    QNetworkReply* authRequest(const QByteArray& postData, const QUrl& url);


private:
    QNetworkAccessManager manager; //used to be a pointer not sure why
};
