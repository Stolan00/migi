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
}
// --------------------------------------------------------------------------------------------------------------------------
QMap<QString, QVariant> Anime::asMap() const {
    QMap<QString, QVariant> mediaValues {
        { "id", id },
        { "titleRomaji",  titleRomaji  },
        { "titleEnglish", titleEnglish },
        { "titleNative",  titleNative  },
        { "synopsis",     synopsis     },
        { "imageLink",    imageLink    },
        { "episodes",     episodes     }
    };

    return mediaValues;
}
// --------------------------------------------------------------------------------------------------------------------------
