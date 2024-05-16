#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSettings>
//i dont know if this class needs to be a singleton or not, i assume no but maybe it should
class Settings : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QSettings m_settings;

public:
    explicit Settings(QObject *parent = nullptr);

    bool setValue(const QAnyStringView key, const QVariant& value);
    QVariant value(QAnyStringView key) const;

signals:
};

