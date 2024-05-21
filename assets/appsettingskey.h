#pragma once

#include <QString>
#include <QMap>

enum class AppSettingsKey {
    // Account settings
    AccountAnilistToken,
    AccountAnilistViewerId,

    // Display settings
    DisplayTheme,
    DisplayLanguage,

    // Network settings
    NetworkAutoSync,

};

// Function to convert AppSettingKey to QString
QString settingsToString(AppSettingsKey key);
