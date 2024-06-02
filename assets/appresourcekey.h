#pragma once

#include <QString>
#include <QMap>

enum class AppResourceKey {
    // gql files
    ALQueryMedia,
    ALQueryMediaFields,
    ALQueryMLCollection,
    ALQueryMLFields,
    ALQueryViewerId,
    ALQueryViewerName,

    // SQL files
    CreateAnimeTable
};

// Function to convert AppSettingKey to QString
QString toString(AppResourceKey key);
