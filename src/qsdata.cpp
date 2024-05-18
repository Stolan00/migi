#include "assets/qsdata.h"

QSData::QSData(QObject *parent)
    : QObject{parent}
{
    animeInfo = "query ($id: Int) {\
        Media(id: $id, type: ANIME) {\
            title {\
                romaji\
                english\
            }\
            startDate {\
                year\
                month\
                day\
            }\
            endDate {\
                year\
                month\
                day\
            }\
            coverImage {\
                large\
            }\
            bannerImage\
            format\
            status\
            episodes\
            season\
            description\
            averageScore\
            meanScore\
            genres\
            synonyms\
            nextAiringEpisode {\
            airingAt\
            timeUntilAiring\
            episode\
            }\
        }\
    }";

    viewerId = "query{\
        Viewer{\
            id\
        }\
    }";
}
