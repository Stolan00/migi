#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QMutex>
#include "appsettingskey.h"

//i dont know if this class needs to be a singleton or not, i assume no but maybe it should
class Settings : public QObject
{
    Q_OBJECT
    QML_SINGLETON

public:
    static Settings& instance();
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    // explicit Settings(QObject *parent = nullptr);

    //TODO: these should probably be slots instead
    static bool setValue(const AppSettingsKey key, const QVariant& value);
    static QVariant value(const AppSettingsKey key);

    static QObject* create(QQmlEngine *engine, QJSEngine *scriptEngine);

signals:

private:
    Settings();
    ~Settings();

    static QMutex mutex; // thread safety
};

