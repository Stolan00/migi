#include "assets/filewriter.h"
// --------------------------------------------------------------------------------------------------------------------------
FileWriter::FileWriter(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<QString>();
}
// --------------------------------------------------------------------------------------------------------------------------
void FileWriter::writeFile(const QString path, const QString value) {
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Could not open file for writing");
        return;
    }

    QTextStream out(&file);

    out << value;

    qDebug() << "File written";

    file.close();
}
// --------------------------------------------------------------------------------------------------------------------------
QString FileWriter::readFile(const QString& path) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "could not open file";

    QTextStream in(&file);
    QString contents = in.readAll();

    file.close();

    return contents;
}
// --------------------------------------------------------------------------------------------------------------------------
