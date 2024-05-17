#include "assets/oauthimplicit.h"

OAuth2ImplicitGrant::OAuth2ImplicitGrant(QObject *parent)
    : QAbstractOAuth2(parent) {
    connect(this, &QAbstractOAuth::authorizeWithBrowser,
            &QDesktopServices::openUrl);

    setAuthorizationUrl(QUrl("https://anilist.co/api/v2/oauth/authorize"));//?client_id=18259&response_type=token"));
    setClientIdentifier("18259");
    setResponseType("token");
    //setScope("email");
    //setRedirectUri(QUrl("http://localhost/callback"));
}

void OAuth2ImplicitGrant::grant() {
    QUrl url = authorizationUrl();
    QUrlQuery query;

    query.addQueryItem("response_type", responseType());
    query.addQueryItem("client_id", clientIdentifier());
    //query.addQueryItem("redirect_uri", redirectUri().toString());
    //query.addQueryItem("scope", scope());
    query.addQueryItem("state", state());

    url.setQuery(query);
    emit authorizeWithBrowser(url);
}
