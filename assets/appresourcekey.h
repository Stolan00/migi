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
    CreateEntryStatusTable,
    CreateEntryTable,
    CreateMediaFormatTable,
    CreateGenreTable,
    CreateModifiedTrigger,
    CreateStatusTable,
    CreateStudioTable,
    CreateAnimeStudioTable
};


// Function to convert AppSettingKey to QString
QString toString(AppResourceKey key);
