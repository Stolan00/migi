#pragma once

#include "qqmlintegration.h"
#include "networkmanager.h"
#include "qsdata.h"
#include <QObject>


class Anilist : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    NetworkManager m_netRequest;
    QSData strings;

    QUrl m_anilist_url;

    int i = 5;

public:
//     //explicit anilist(QObject *parent = nullptr);
    Anilist();
    void configureOAuth2();

signals:
    void responseReceived(const QJsonObject &response);

public slots:
    void search_anime();
};
