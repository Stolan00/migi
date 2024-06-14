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

        // SQL Tables
        { AppResourceKey::CreateAnimeTable,        ":/assets/sql/tables/CreateAnimeTable.sql"        },
        { AppResourceKey::CreateAnimeGenreTable,   ":/assets/sql/tables/CreateAnimeGenreTable.sql"   },
        { AppResourceKey::CreateAnimeStatusTable,  ":/assets/sql/tables/CreateAnimeStatusTable.sql"  },
        { AppResourceKey::CreateEntryStatusTable,  ":/assets/sql/tables/CreateEntryStatusTable.sql"  },
        { AppResourceKey::CreateEntryTable,        ":/assets/sql/tables/CreateEntryTable.sql"        },
        { AppResourceKey::CreateMediaFormatTable,  ":/assets/sql/tables/CreateMediaFormatTable.sql"  },
        { AppResourceKey::CreateGenreTable,        ":/assets/sql/tables/CreateGenreTable.sql"        },
        { AppResourceKey::CreateStatusTable,       ":/assets/sql/tables/CreateStatusTable.sql"       },
        { AppResourceKey::CreateStudioTable,       ":/assets/sql/tables/CreateStudioTable.sql"       },
        { AppResourceKey::CreateAnimeStudioTable,  ":/assets/sql/tables/CreateAnimeStudioTable.sql"  },
        { AppResourceKey::SQLPopulateTables,       ":assets/sql/queries/PopulateTables.sql"          },

        //SQL Triggers
        { AppResourceKey::CreateModifiedTrigger,   ":/assets/sql/triggers/CreateModifiedTrigger.sql" },

        //SQL Queries
        { AppResourceKey::FindUpdatedEntries, ":/assets/sql/queries/FindUpdatedEntries.sql" }
    };

    return keyToString.value(key, "");
}
// --------------------------------------------------------------------------------------------------------------------------
