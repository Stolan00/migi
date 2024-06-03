#include "filewriter.h"
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
QStringList FileWriter::readFilesFromDirectory(const QString& directoryPath, const QStringList& fileExtensions) {
    QDir dir(directoryPath);
    QStringList filter;

    for (const QString &extension : fileExtensions) {
        filter << QString("*.%1").arg(extension);
    }

    QStringList files = dir.entryList(filter, QDir::Files);
    QStringList fileContents;

    for (const QString &file : files) {
        QFile fileHandle(dir.absoluteFilePath(file));
        if (fileHandle.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&fileHandle);
            fileContents.append(in.readAll());
            fileHandle.close();
        } else {
            qWarning() << "Failed to open file:" << fileHandle.fileName();
        }
    }

    return fileContents;
}

// --------------------------------------------------------------------------------------------------------------------------
// Helper function for passing in a single extension
QStringList FileWriter::readFilesFromDirectory(const QString& directoryPath, const QString& fileExtension) {
    return readFilesFromDirectory(directoryPath, QStringList() << fileExtension);
}
