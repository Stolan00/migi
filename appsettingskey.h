#pragma once

#include <QString>
#include <QMap>

enum class AppSettingsKey {
    // Account settings
    AccountAnilistToken,

    // Display settings
    DisplayTheme,
    DisplayLanguage,

    // Network settings
    NetworkAutoSync,

};

// Function to convert AppSettingKey to QString
QString toString(AppSettingsKey key);
