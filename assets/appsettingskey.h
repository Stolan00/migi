#pragma once

#include <QString>
#include <QMap>

enum class AppSettingsKey {
    // Account settings
    AccountAnilistToken,
    AccountAnilistViewerId,
    AccountAnilistViewerName,

    // Display settings
    DisplayTheme,
    DisplayLanguage,

    // Network settings
    NetworkAutoSync,

    // Database settings
    DatabasePath

};

// Function to convert AppSettingKey to QString
QString toString(AppSettingsKey key);

