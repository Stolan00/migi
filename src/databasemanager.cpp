#include "databasemanager.h"
// --------------------------------------------------------------------------------------------------------------------------
DatabaseManager::DatabaseManager(const QString &path)
{
    createConnection(path);
}
// --------------------------------------------------------------------------------------------------------------------------
DatabaseManager::~DatabaseManager() {
    m_database.close();
}
// --------------------------------------------------------------------------------------------------------------------------
bool DatabaseManager::createConnection(const QString& path) {
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(path);

    if (!m_database.open()) {
        qDebug() << "Error: Unable to open database.";
        return false;
    }

    return true;
}
// --------------------------------------------------------------------------------------------------------------------------
bool DatabaseManager::createTable(const QString& createTableQuery) {
    QSqlQuery query;

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error: Unable to create table." << query.lastError();
        return false;
    }
    return true;
}
// --------------------------------------------------------------------------------------------------------------------------
bool DatabaseManager::insertIntoTable(const QString& tableName, const QVariantMap& values) {
    QSqlQuery query;
    QStringList columns = values.keys();
    QStringList placeholders;

    for (const QString& column : columns) {
        placeholders << ":" + column;
    }

    QString insertQuery = "INSERT INTO " + tableName + " ("
                          + columns.join(", ") + ") VALUES (" + placeholders.join(", ") + ");";

    query.prepare(insertQuery);

    for (const QString& column : columns) {
        query.bindValue(":" + column, values.value(column));
    }

    if (!query.exec()) {
        qDebug() << "Error: Unable to insert data." << query.lastError();
        return false;
    }
    return true;
}
// --------------------------------------------------------------------------------------------------------------------------
