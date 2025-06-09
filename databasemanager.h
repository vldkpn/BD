#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    bool openConnection(const QString& host, const QString& dbName, const QString& user, const QString& password);
    void closeConnection();
    QSqlDatabase database() const;

private:
    DatabaseManager();
    QSqlDatabase m_database;
};

#endif // DATABASEMANAGER_H
