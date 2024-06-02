#include "assets/appresourcekey.h"
// --------------------------------------------------------------------------------------------------------------------------
QString toString(AppResourceKey key) {
    static const QMap<AppResourceKey, QString> keyToString {

        // Queries
        { AppResourceKey::ALQueryMedia,        ":/assets/gql/Media.gql"               },
        { AppResourceKey::ALQueryMediaFields,  ":/assets/gql/MediaFields.gql"         },
        { AppResourceKey::ALQueryMLCollection, ":/assets/gql/MediaListCollection.gql" },
        { AppResourceKey::ALQueryMLFields,     ":/assets/gql/MediaListFields.gql"     },
        { AppResourceKey::ALQueryViewerId,     ":/assets/gql/ViewerId.gql"            },
        { AppResourceKey::ALQueryViewerName,   ":/assets/gql/ViewerName.gql"          },

        // SQL Queries
        { AppResourceKey::CreateAnimeTable, ":/assets/sql/CreateAnimeTable.sql" },
    };

    return keyToString.value(key, "");
}
// --------------------------------------------------------------------------------------------------------------------------
