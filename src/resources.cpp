#include "assets/resources.h"
#include "qdebug.h"
#include "qvariant.h"
#include <QFile>
#include <QTextStream>

Resources::Resources(QObject *parent)
    : QObject{parent}
{}

// --------------------------------------------------------------------------------------------------------------------------
QString Resources::value(const AppResourceKey key) {
    return toString(key);
}
// --------------------------------------------------------------------------------------------------------------------------
// Helper function for reading resources
QVariant Resources::readResource(const AppResourceKey& key) {
    return readResourceFile( value(key) );
}
// --------------------------------------------------------------------------------------------------------------------------
QVariant Resources::readResourceFile(const QString &resourcePath) { //TODO: need to handle multiple resource types
    QFile file(resourcePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open resource file:" << resourcePath;
        return QString();
    }

    QTextStream in(&file);
    QString content = in.readAll();
    content.replace('\n', ' ').replace("\r", "");
    content = content.simplified();

    qDebug() << "RESOURCES CONTENT: " << content;

    return content;
}
// --------------------------------------------------------------------------------------------------------------------------
