#include "assets/appsettingskey.h"
#include "qdebug.h"
// --------------------------------------------------------------------------------------------------------------------------
QString toString(AppSettingsKey key) {
    static const QMap<AppSettingsKey, QString> keyToString {

        // Acount
        { AppSettingsKey::AccountAnilistToken,      "account/anilist/token"    },
        { AppSettingsKey::AccountAnilistViewerId,   "account/anilist/viewerId" },
        { AppSettingsKey::AccountAnilistViewerName, "account/anilist/viewerName" },

        // Display
        { AppSettingsKey::DisplayTheme,    "display/theme"    },
        { AppSettingsKey::DisplayLanguage, "display/language" },

        // Networking
        { AppSettingsKey::NetworkAutoSync, "network/autoSync" },

        // Database
        { AppSettingsKey::DatabasePath, "database/name" }

    };

    return keyToString.value(key, "");
}
// --------------------------------------------------------------------------------------------------------------------------
