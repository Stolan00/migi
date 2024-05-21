#include "assets/appsettingskey.h"
#include "qdebug.h"
// --------------------------------------------------------------------------------------------------------------------------
QString settingsToString(AppSettingsKey key) {
    static const QMap<AppSettingsKey, QString> keyToString {

        // Acount
        { AppSettingsKey::AccountAnilistToken,    "account/anilist/token"    },
        { AppSettingsKey::AccountAnilistViewerId, "account/anilist/viewerId" },

        // Display
        { AppSettingsKey::DisplayTheme,    "display/theme"    },
        { AppSettingsKey::DisplayLanguage, "display/language" },

        // Networking
        { AppSettingsKey::NetworkAutoSync, "network/autoSync" }

    };

    return keyToString.value(key, "");
}
// --------------------------------------------------------------------------------------------------------------------------
