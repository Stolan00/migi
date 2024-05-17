#include "assets/appsettingskey.h"

QString toString(AppSettingsKey key) {
    static const QMap<AppSettingsKey, QString> keyToString {

        // Acount
        { AppSettingsKey::AccountAnilistToken, "account/anilist/token" },

        // Display
        { AppSettingsKey::DisplayTheme, "display/theme" },
        { AppSettingsKey::DisplayLanguage, "display/language" },
        { AppSettingsKey::NetworkAutoSync, "network/autoSync" }


    };

    return keyToString.value(key, "");
}
