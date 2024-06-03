#pragma once

#include <QString>
#include <QMap>

enum class AppResourceKey {
    // GraphQL Queries
    ALQueryMedia,
    ALQueryMediaFields,
    ALQueryMLCollection,
    ALQueryMLFields,
    ALQueryViewerId,
    ALQueryViewerName,

    // SQL Queries
    CreateAnimeTable,
    CreateAnimeGenreTable,
    CreateAnimeStatusTable,
    CreateAnimeSynonymTable,
    CreateEntryStatusTable,
    CreateEntryTable,
    CreateFormatTable,
    CreateGenreTable,
    CreateModifiedTrigger,
    CreateStatusTable
};


// Function to convert AppSettingKey to QString
QString toString(AppResourceKey key);
