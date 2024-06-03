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
QStringList DatabaseManager::getAllTables() {
    QStringList tables;

    if (m_database.isOpen()) {
        QSqlQuery query("SELECT name FROM sqlite_master WHERE type='table';", m_database);
        while (query.next()) {
            tables << query.value(0).toString();
        }
    } else {
        qWarning() << "Database is not open!";
    }

    return tables;
}
// --------------------------------------------------------------------------------------------------------------------------
QStringList DatabaseManager::getTableSchema(const QString& tableName) {
    QStringList schema;

    if (m_database.isOpen()) {
        QSqlQuery query(m_database);
        query.prepare(QString("PRAGMA table_info(%1);").arg(tableName));
        if (query.exec()) {
            while (query.next()) {
                QString columnName = query.value("name").toString();
                QString columnType = query.value("type").toString();
                schema << QString("%1 %2").arg(columnName, columnType);
            }
        } else {
            qWarning() << "Failed to execute PRAGMA table_info:" << query.lastError();
        }
    } else {
        qWarning() << "Database is not open!";
    }

    return schema;
}
// --------------------------------------------------------------------------------------------------------------------------
bool DatabaseManager::deleteAllTables() {
    QStringList tables = getAllTables();

    foreach (const QString& table, tables) {
        QSqlQuery dropQuery(m_database);

        QString dropStatement = QString("DROP TABLE IF EXISTS %1;").arg(table);

        if (!dropQuery.exec(dropStatement)) {
            qDebug() << "Failed to drop table" << table << ":" << dropQuery.lastError();
            return false;
        } else {
            qDebug() << "Dropped table:" << table;
        }
    }

    qDebug() << "ALL TABLES DELETED";
    return true;
}
