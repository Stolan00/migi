#pragma once
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include "filewriter.h"
#include "qmutex.h"
#include "settings.h"
#include <QObject>

class DatabaseManager : public QObject
{
    Q_OBJECT
    QSqlDatabase m_database;

public:
    static DatabaseManager& instance();
    bool executeQuery(const QString& queryStr);
    bool createConnection(const QString& path);
    bool createTable(const QString& createTableQuery);
    bool insertIntoTable(const QString& tableName, const QVariantMap& values);

    bool bulkInsertIntoTable(const QString& tableName, const QList<QHash<QString, QVariant> > &valuesList);
    bool printAllValuesFromTable(const QString& tableName);
    QStringList getAllTables();
    QStringList getTableSchema(const QString& tableName);
    QStringList getColumnNames(const QString& tableName);

    void executeSqlScript(const QString &filePath);
    void executeQueriesFromFile(QFile *file, QSqlQuery *query);

    bool deleteAllTables(); // TODO: probably shouldnt be public, leaving for development
signals:

private:
    DatabaseManager(const QString& path);
    ~DatabaseManager();
    Settings& m_settings = Settings::instance();

    static QMutex mutex;
};
