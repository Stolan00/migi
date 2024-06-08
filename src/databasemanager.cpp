#include "databasemanager.h"

// --------------------------------------------------------------------------------------------------------------------------
DatabaseManager::DatabaseManager(const QString &path) {
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
bool DatabaseManager::printAllValuesFromTable(const QString& tableName) {
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return false;
    }

    QSqlQuery query;
    QString selectQuery = QString("SELECT * FROM %1").arg(tableName);

    if (!query.exec(selectQuery)) {
        qDebug() << "Error: Failed to execute query -" << query.lastError();
        return false;
    }

    QSqlRecord record = query.record();
    int fieldCount = record.count();

    while (query.next()) {
        QStringList rowValues;
        for (int i = 0; i < fieldCount; ++i) {
            rowValues << query.value(i).toString();
        }
        qDebug() << rowValues.join(", ");
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

    qDebug() << "INSERTED SUCCESSFULLY";

    return true;
}
// --------------------------------------------------------------------------------------------------------------------------
bool DatabaseManager::bulkInsertIntoTable(const QString& tableName, const QList<QHash<QString, QVariant>>& valuesList) {
    if (valuesList.isEmpty()) {
        return true; // Nothing to insert
    }

    QSqlQuery query;

    // Begin transaction
    if (!query.exec("BEGIN TRANSACTION")) {
        qDebug() << "Error: Failed to begin transaction." << query.lastError();
        return false;
    }

    // Ensure columns are in the correct order according to the schema
    QStringList orderedColumns = getColumnNames(tableName);
    QStringList placeholders;

    for (const QString& column : orderedColumns) {
        placeholders << "?";
    }

    QString insertQuery = "INSERT INTO " + tableName + " ("
                          + orderedColumns.join(", ") + ") VALUES ";

    QStringList valuesPlaceholderGroups;
    for (int i = 0; i < valuesList.size(); ++i) {
        valuesPlaceholderGroups << "(" + placeholders.join(", ") + ")";
    }

    insertQuery += valuesPlaceholderGroups.join(", ");

    qDebug() << "Bulk Insert Query: " << insertQuery;
    if (!query.prepare(insertQuery)) {
        qDebug() << "Error: Failed to prepare bulk insert query." << query.lastError();
        return false;
    }

    // Bind values for all rows
    int bindIndex = 0;
    for (const QHash<QString, QVariant>& values : valuesList) {
        for (const QString& column : orderedColumns) {
            query.bindValue(bindIndex++, values.value(column));
        }
    }

    // Execute the bulk insert query
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute bulk insert query:" << query.lastError();
        if (!query.exec("ROLLBACK")) {
            qDebug() << "Error: Failed to rollback transaction." << query.lastError();
        }
        return false;
    }

    // Commit transaction
    if (!query.exec("COMMIT")) {
        qDebug() << "Error: Failed to commit transaction." << query.lastError();
        return false;
    }

    qDebug() << "Bulk insert successful.";
    return true;
}
// --------------------------------------------------------------------------------------------------------------------------
QStringList DatabaseManager::getAllTables() {
    QStringList tables;

    if (!m_database.isOpen()) {
        qWarning() << "Database is not open!";
        return tables;
    }

    QSqlQuery query("SELECT name FROM sqlite_master WHERE type='table';", m_database);
    while (query.next()) {
        tables << query.value(0).toString();
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
QStringList DatabaseManager::getColumnNames(const QString& tableName) {
    QStringList columnNames;
    QSqlQuery query;

    // Prepare the query to get column names
    query.prepare("PRAGMA table_info(" + tableName + ");");

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query to get column names." << query.lastError();
        return columnNames;
    }

    // Iterate through the result set to collect column names
    while (query.next()) {
        columnNames << query.value("name").toString();
    }

    return columnNames;
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
