#pragma once

#include <QAbstractOAuth>
#include <QOAuth2AuthorizationCodeFlow>
#include <QDesktopServices>
#include <QUrl>
#include <QUrlQuery>

class OAuth2ImplicitGrant : public QAbstractOAuth2 {
    Q_OBJECT

public:
    explicit OAuth2ImplicitGrant(QObject *parent = nullptr);
    void configure(const QString& clientId, const QUrl& authUrl, const QUrl& accessUrl);

public slots:
    void grant();

private:
    QOAuth2AuthorizationCodeFlow oauth;
};

