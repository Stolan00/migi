#pragma once


#include <QJsonObject>
#include <QJsonArray>

class Anime
{

public:
    Anime();
    Anime(const QJsonObject jsonValues);

        //TODO: put in 'Media' ABC? (if going to allow manga)
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

    enum class MediaFormat { //TODO: probably doesnt belong here
        TV,
        TV_SHORT,
        MOVIE,
        SPECIAL,
        OVA,
        ONA,
        MUSIC,
        MANGA,
        NOVEL,
        ONE_SHOT
    };

    struct ListEntryInfo {
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

        int anilistModified;
    };

    struct Studio {
        int studioId;
        QString studioName;
        bool isMain;

        Studio(int newId, QString newName, bool newIsMain) {
            studioId = newId;
            studioName = newName;
            isMain = newIsMain;
        }
    };

    QHash<QString, QVariant> asHash() const;
    QHash<QString, QVariant> myInfoAsHash() const;
    int getGenreIndex(const QString& genre) const;

    // Attributes
    int id, idMal;
    int episodes;
    int anilistModified;

    QString titleRomaji;
    QString titleEnglish;
    QString titleNative;
    QString synopsis;
    QString imageLink;

    MediaStatus   status;
    ListEntryInfo myInfo;
    MediaFormat   format;

    QStringList   genres;
    QList<Studio> studios;

signals:

private:
    MediaStatus toMediaStatusEnum(const QString& status);
    EntryStatus toEntryStatusEnum(const QString& status);
    MediaFormat toMediaFormatEnum(const QString& format);
};
