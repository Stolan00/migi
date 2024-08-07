#include <anime.h>

// --------------------------------------------------------------------------------------------------------------------------
Anime::Anime() {
}
// --------------------------------------------------------------------------------------------------------------------------
Anime::Anime(const QJsonObject jsonValues) {
    bool isEntry = jsonValues.contains("media"); //TODO: need more checks here

    QJsonObject animeValues;

    if (isEntry) {
        myInfo.id = jsonValues["id"].toInt();

        QJsonObject completedInfo = jsonValues["completedAt"].toObject();

        myInfo.completedDay   = completedInfo["day"].toInt();
        myInfo.completedMonth = completedInfo["month"].toInt();
        myInfo.completedYear  = completedInfo["year"].toInt();

        QJsonObject startedInfo = jsonValues["startedAt"].toObject();

        myInfo.startedDay   = startedInfo["day"].toInt();
        myInfo.startedMonth = startedInfo["month"].toInt();
        myInfo.startedYear  = startedInfo["year"].toInt();

        myInfo.progress = jsonValues["progress"].toInt();
        myInfo.repeat   = jsonValues["repeat"].toInt();
        myInfo.score    = jsonValues["score"].toInt();

        myInfo.status = toEntryStatusEnum( jsonValues["status"].toString() );

        myInfo.notes  = jsonValues["notes"].toString();

        myInfo.isPrivate = jsonValues["private"].toBool();

        myInfo.anilistModified = jsonValues["updatedAt"].toInt();

        animeValues = jsonValues["media"].toObject();
    }

    else {
        animeValues = jsonValues;
    }

    id = animeValues["id"].toInt();
    idMal = animeValues["idMal"].toInt();

    myInfo.isFavourite = animeValues["isFavourite"].toBool();

    QJsonObject animeTitle = animeValues["title"].toObject();
    titleRomaji  = animeTitle["romaji"].toString();
    titleEnglish = animeTitle["english"].toString();
    titleNative  = animeTitle["native"].toString();


    synopsis = animeValues["description"].toString();

    QJsonObject animeImage = animeValues["coverImage"].toObject();
    imageLink = animeImage["large"].toString();

    bannerImageLink = animeValues["bannerImage"].toString();

    QJsonArray animeSynonyms = animeValues["synonyms"].toArray();

    for (const QJsonValue &synonym : animeSynonyms) {
        synonyms.append( synonym.toString() );
    }

    episodes = animeValues["episodes"].toInt();

    status = toMediaStatusEnum( animeValues["status"].toString() );

    anilistModified = animeValues["updatedAt"].toInt();

    format = toMediaFormatEnum( animeValues["format"].toString() );

    seasonYear = animeValues["seasonYear"].toInt();
    season = toMediaSeasonEnum( animeValues["season"].toString() );

    QJsonArray genreArray = animeValues["genres"].toArray();

    for (const QJsonValue& value : genreArray) {
        genres.append(value.toString());
    }

    QJsonObject studiosObj = animeValues["studios"].toObject();
    QJsonArray animeStudios = studiosObj["edges"].toArray();

    for (const QJsonValue& value : animeStudios) {
        QJsonObject edgeObj = value.toObject();
        bool studioIsMain = edgeObj["isMain"].toBool();
        QJsonObject nodeObj = edgeObj["node"].toObject();
        QString studioName = nodeObj["name"].toString();
        int studioId = nodeObj["id"].toInt();

        Studio newStudio(studioId, studioName, studioIsMain);

        //qDebug() << titleEnglish << " " << " " << titleRomaji << " " << newStudio.studioId << " " << newStudio.studioName << " " << newStudio.isMain;

        studios.append(newStudio);
    }
}
// --------------------------------------------------------------------------------------------------------------------------
QHash<QString, QVariant> Anime::asHash() const {
    QHash<QString, QVariant> mediaValues {
        { "id", id },
        { "idMal", idMal },
        { "titleRomaji",  titleRomaji  },
        { "titleEnglish", titleEnglish },
        { "titleNative",  titleNative  },
        { "synopsis",     synopsis     },
        { "imageLink",    imageLink    },
        { "episodes",     episodes     },
        { "seasonYear",   seasonYear   },
        { "anilistModified", anilistModified },
        { "bannerImageLink",    bannerImageLink },
        { "mediaStatus", static_cast<int>(status) },
        { "mediaFormat", static_cast<int>(format) },
        { "season", static_cast<int>(season) },
    };

    return mediaValues;
}
// --------------------------------------------------------------------------------------------------------------------------
QHash<QString, QVariant> Anime::myInfoAsHash() const {
    QHash<QString, QVariant> myInfoValues {
        { "id", myInfo.id  },
        { "mediaId",    id },
        { "completedDay",    myInfo.completedDay     },
        { "completedMonth",  myInfo.completedMonth   },
        { "completedYear",   myInfo.completedYear    },
        { "startedDay",      myInfo.startedDay       },
        { "startedMonth",    myInfo.startedMonth     },
        { "startedYear",     myInfo.startedYear      },
        { "isPrivate",       myInfo.isPrivate },
        { "anilistModified", myInfo.anilistModified  },
        { "progress", myInfo.progress },
        { "repeat",   myInfo.repeat   },
        { "score",    myInfo.score    },
        { "notes",    myInfo.notes    },
        { "status", static_cast<int>(myInfo.status) },
        { "private", myInfo.isPrivate },
        { "isFavourite", myInfo.isFavourite },
    };

    return myInfoValues;
}

//TODO: put in 'Media' ABC as template function?
// --------------------------------------------------------------------------------------------------------------------------
Anime::MediaStatus Anime::toMediaStatusEnum(const QString& status) {
    if (status == "FINISHED")  return MediaStatus::FINISHED;
    if (status == "RELEASING") return MediaStatus::RELEASING;
    if (status == "CANCELLED") return MediaStatus::CANCELLED;
    if (status == "HIATUS")    return MediaStatus::HIATUS;
    if (status == "NOT_YET_RELEASED") return MediaStatus::NOT_YET_RELEASED;

    return MediaStatus::FINISHED;
}
// --------------------------------------------------------------------------------------------------------------------------
Anime::EntryStatus Anime::toEntryStatusEnum(const QString& status) {
    if (status == "CURRENT")   return EntryStatus::CURRENT;
    if (status == "PLANNING")  return EntryStatus::PLANNING;
    if (status == "COMPLETED") return EntryStatus::COMPLETED;
    if (status == "DROPPED")   return EntryStatus::DROPPED;
    if (status == "PAUSED")    return EntryStatus::PAUSED;
    if (status == "REPEATING") return EntryStatus::REPEATING;

    return EntryStatus::PLANNING;
}
// --------------------------------------------------------------------------------------------------------------------------
QString Anime::toEntryStatusString(Anime::EntryStatus status) {
    switch (status) {
    case EntryStatus::CURRENT:
        return "CURRENT";
    case EntryStatus::PLANNING:
        return "PLANNING";
    case EntryStatus::COMPLETED:
        return "COMPLETED";
    case EntryStatus::DROPPED:
        return "DROPPED";
    case EntryStatus::PAUSED:
        return "PAUSED";
    case EntryStatus::REPEATING:
        return "REPEATING";
    default:
        return "PLANNING";  // Default case, can be adjusted if needed
    }
}
// --------------------------------------------------------------------------------------------------------------------------
Anime::MediaSeason Anime::toMediaSeasonEnum(const QString& status) {
    if (status == "WINTER") return MediaSeason::WINTER;
    if (status == "SPRING") return MediaSeason::SPRING;
    if (status == "SUMMER") return MediaSeason::SUMMER;
    if (status == "FALL")   return MediaSeason::FALL;

    return MediaSeason::INVALID;
}
// --------------------------------------------------------------------------------------------------------------------------
Anime::MediaFormat Anime::toMediaFormatEnum(const QString& format) {
    if (format == "TV")       return MediaFormat::TV;
    if (format == "TV_SHORT") return MediaFormat::TV_SHORT;
    if (format == "MOVIE")    return MediaFormat::MOVIE;
    if (format == "SPECIAL")  return MediaFormat::SPECIAL;
    if (format == "OVA")      return MediaFormat::OVA;
    if (format == "ONA")      return MediaFormat::ONA;
    if (format == "MUSIC")    return MediaFormat::MUSIC;
    if (format == "MANGA")    return MediaFormat::MANGA;
    if (format == "NOVEL")    return MediaFormat::NOVEL;
    if (format == "ONE_SHOT") return MediaFormat::ONE_SHOT;

    // Default case if no match
    return MediaFormat::TV;
}
// --------------------------------------------------------------------------------------------------------------------------
int Anime::getGenreIndex(const QString& genre) const{ //TODO: probably shouldnt go here
    static const QMap<QString, int> genreMap = {
        {"Action", 0},
        {"Adventure", 1},
        {"Comedy", 2},
        {"Drama", 3},
        {"Ecchi", 4},
        {"Fantasy", 5},
        {"Hentai", 6},
        {"Horror", 7},
        {"Mahou Shoujo", 8},
        {"Mecha", 9},
        {"Music", 10},
        {"Mystery", 11},
        {"Psychological", 12},
        {"Romance", 13},
        {"Sci-Fi", 14},
        {"Slice of Life", 15},
        {"Sports", 16},
        {"Supernatural", 17},
        {"Thriller", 18}
    };

    if (genreMap.contains(genre)) {
        return genreMap.value(genre);
    } else {
        return -1; // Genre not found
    }
}
