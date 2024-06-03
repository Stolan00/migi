#include "assets/appresourcekey.h"
// --------------------------------------------------------------------------------------------------------------------------
QString toString(AppResourceKey key) {
    static const QMap<AppResourceKey, QString> keyToString {

       // GraphQL Queries
       { AppResourceKey::ALQueryMedia,        ":/assets/gql/Media.gql"               },
       { AppResourceKey::ALQueryMediaFields,  ":/assets/gql/MediaFields.gql"         },
       { AppResourceKey::ALQueryMLCollection, ":/assets/gql/MediaListCollection.gql" },
       { AppResourceKey::ALQueryMLFields,     ":/assets/gql/MediaListFields.gql"     },
       { AppResourceKey::ALQueryViewerId,     ":/assets/gql/ViewerId.gql"            },
       { AppResourceKey::ALQueryViewerName,   ":/assets/gql/ViewerName.gql"          },

       // SQL Queries
       { AppResourceKey::CreateAnimeTable,        ":/assets/sql/CreateAnimeTable.sql"        },
       { AppResourceKey::CreateAnimeGenreTable,   ":/assets/sql/CreateAnimeGenreTable.sql"   },
       { AppResourceKey::CreateAnimeStatusTable,  ":/assets/sql/CreateAnimeStatusTable.sql"  },
       { AppResourceKey::CreateAnimeSynonymTable, ":/assets/sql/CreateAnimeSynonymTable.sql" },
       { AppResourceKey::CreateEntryStatusTable,  ":/assets/sql/CreateEntryStatusTable.sql"  },
       { AppResourceKey::CreateEntryTable,        ":/assets/sql/CreateEntryTable.sql"        },
       { AppResourceKey::CreateFormatTable,       ":/assets/sql/CreateFormatTable.sql"       },
       { AppResourceKey::CreateGenreTable,        ":/assets/sql/CreateGenreTable.sql"        },
       { AppResourceKey::CreateModifiedTrigger,   ":/assets/sql/CreateModifiedTrigger.sql"   },
       { AppResourceKey::CreateStatusTable,       ":/assets/sql/CreateStatusTable.sql"       },
       };

    return keyToString.value(key, "");

}
// --------------------------------------------------------------------------------------------------------------------------
