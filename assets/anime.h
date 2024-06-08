#pragma once


#include <QJsonObject>

class Anime
{

public:
    Anime();
    Anime(QJsonObject animeValues);

    QHash<QString, QVariant> asHash() const;

    // Attributes
    int id;
    int episodes;

    QString titleRomaji;
    QString titleEnglish;
    QString titleNative;
    QString synopsis;
    QString imageLink;
signals:
};
