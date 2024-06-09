#pragma once


#include <QJsonObject>

class Anime
{

public:
    Anime();
    Anime(QJsonObject animeValues);

        //TODO: put in 'Media' ABC? (if going to allow manga)
        //or just make it an enum class
    enum class MediaStatus {
        FINISHED,
        RELEASING,
        NOT_YET_RELEASED,
        CANCELLED,
        HIATUS,
    };

    enum class EntryStatus {
        CURRENT,
        PLANNING,
        COMPLETED,
        DROPPED,
        PAUSED,
        REPEATING
    };

    struct listEntryInfo {
        int id;

        int completedDay,
            completedMonth,
            completedYear;

        int startedDay,
            startedMonth,
            startedYear;

        int progress;
        int repeat;
        int score;

        EntryStatus status;

        QString notes;

        bool isPrivate;

        //int modified?
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
