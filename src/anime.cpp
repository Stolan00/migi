#include <anime.h>

// --------------------------------------------------------------------------------------------------------------------------
Anime::Anime()
{

}
// --------------------------------------------------------------------------------------------------------------------------
Anime::Anime(QJsonObject animeValues) {

}
// --------------------------------------------------------------------------------------------------------------------------
QMap<QString, QVariant> Anime::asMap() {
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
