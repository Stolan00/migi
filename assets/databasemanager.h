#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

#include <QObject>

class DatabaseManager : public QObject
{
    Q_OBJECT
    QSqlDatabase m_database;

public:
    DatabaseManager(const QString& path);
    bool createConnection(const QString& path);
    bool createTable(const QString& createTableQuery);
    bool insertIntoTable(const QString& tableName, const QVariantMap& values);
    ~DatabaseManager();

signals:
};

#endif // DATABASEMANAGER_H
