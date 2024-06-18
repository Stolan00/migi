#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QMutex>
#include "appsettingskey.h"

//TODO: probably doesnt need to be singleton
class Settings : public QObject
{
    Q_OBJECT
    QML_SINGLETON

public:
    static Settings& instance();
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    // explicit Settings(QObject *parent = nullptr);

    static QObject* create(QQmlEngine *engine, QJSEngine *scriptEngine);

public slots:
    static bool setValue(const AppSettingsKey key, const QVariant& value);
    static QVariant value(const AppSettingsKey key);

signals:

private:
    Settings();
    ~Settings();

    static QMutex mutex; // thread safety
};

