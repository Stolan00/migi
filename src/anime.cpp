#include <anime.h>

// --------------------------------------------------------------------------------------------------------------------------
Anime::Anime()
{

}
// --------------------------------------------------------------------------------------------------------------------------
Anime::Anime(QJsonObject animeValues) {
    id = animeValues["id"].toInt();

    QJsonObject animeTitle = animeValues["title"].toObject();
    titleRomaji  = animeTitle["romaji"].toString();
    titleEnglish = animeTitle["english"].toString();
    titleNative  = animeTitle["native"].toString();

    synopsis = animeValues["description"].toString();

    QJsonObject animeImage = animeValues["coverImage"].toObject();
    imageLink = animeImage["large"].toString();

    episodes = animeValues["episodes"].toInt();

    status = toEnum( animeValues["status"].toString() );
}
// --------------------------------------------------------------------------------------------------------------------------
QHash<QString, QVariant> Anime::asHash() const {
    QHash<QString, QVariant> mediaValues {
        { "id", id },
        { "titleRomaji",  titleRomaji  },
        { "titleEnglish", titleEnglish },
        { "titleNative",  titleNative  },
        { "synopsis",     synopsis     },
        { "imageLink",    imageLink    },
        { "episodes",     episodes     },
        { "mediaStatus", static_cast<int>(status) }
    };

    return mediaValues;
}
// --------------------------------------------------------------------------------------------------------------------------
Anime::MediaStatus Anime::toEnum(const QString& status) {
    if (status == "FINISHED") {
        return MediaStatus::FINISHED;
    } else if (status == "RELEASING") {
        return MediaStatus::RELEASING;
    } else if (status == "NOT_YET_RELEASED") {
        return MediaStatus::NOT_YET_RELEASED;
    } else if (status == "HIATUS") {
        return MediaStatus::HIATUS;
    } else {
        // Handle unknown status case. You can throw an exception, return a default value, or handle it as needed.
        // For this example, let's return FINISHED as a default value.
        return MediaStatus::FINISHED;
    }
}
