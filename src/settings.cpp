#include "assets/settings.h"

QMutex Settings::mutex;
// --------------------------------------------------------------------------------------------------------------------------
Settings& Settings::instance() {
    QMutexLocker locker(&mutex);
    static Settings instance;
    return instance;
}
// --------------------------------------------------------------------------------------------------------------------------
Settings::Settings() {

}
// --------------------------------------------------------------------------------------------------------------------------
Settings::~Settings() {

}
// --------------------------------------------------------------------------------------------------------------------------
bool Settings::setValue(const AppSettingsKey key, const QVariant& value) {

    QSettings settings;

    settings.setValue( toString( key ), value );

    settings.sync();

    qDebug() << "QSettings file path:" << settings.fileName();

    return settings.contains( toString( key ) );
}
// --------------------------------------------------------------------------------------------------------------------------
QVariant Settings::value(const AppSettingsKey key) {

    QSettings settings;

    QAnyStringView path = toString(key);

    QVariant result = settings.value( path );


    return result.toString();//, settingDoesNotExist ); // Will return null variant if no setting exists, can be checked with .isNull()
                                                                // dont think thats accurate actually
}
// --------------------------------------------------------------------------------------------------------------------------
QObject* Settings::create(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    qDebug() << "Settings::create called";
    return &instance();
}
// --------------------------------------------------------------------------------------------------------------------------
