#pragma once


#include <QJsonObject>

class Anime
{

public:
    Anime();
    Anime(QJsonObject animeValues);

        //TODO: put in 'Media' ABC? (if going to allow manga)
        //or just make it an enum class
    enum MediaStatus {
        FINISHED,
        RELEASING,
        NOT_YET_RELEASED,
        CANCELLED,
        HIATUS,
    };

    QHash<QString, QVariant> asHash() const;

    // Attributes
    int id;
    int episodes;

    QString titleRomaji;
    QString titleEnglish;
    QString titleNative;
    QString synopsis;
    QString imageLink;

    MediaStatus status;
signals:

private:
    MediaStatus toEnum(const QString& status);
};
