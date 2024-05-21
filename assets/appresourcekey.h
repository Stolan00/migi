#pragma once

#include <QString>
#include <QMap>

enum class AppResourceKey {
    // gql files
    ALQueryMedia,
    ALQueryMediaFields,
    ALQueryMLCollection,
    ALQueryMLFields,
    ALQueryViewerId
};

// Function to convert AppSettingKey to QString
QString toString(AppResourceKey key);
