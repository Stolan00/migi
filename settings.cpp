#include "settings.h"

Settings::Settings(QObject *parent)
    : QObject{parent}
{
    qDebug() << "QSettings file path:" << m_settings.fileName();

}

bool Settings::setValue(const QAnyStringView key, const QVariant& value) {
    m_settings.setValue(key, value);

    m_settings.sync();

    qDebug() << "QSettings file path:" << m_settings.fileName();

    return true;
}

QVariant Settings::value(QAnyStringView key) const {

    QVariant settingDoesNotExist; // Default QVariant constructor is null

    return m_settings.value(key, settingDoesNotExist); // Will return null variant if no setting exists, can be checked with .isNull()
}
