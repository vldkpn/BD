#include "databasemanager.h"
#include <QtSql/QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager()
{
    m_database = QSqlDatabase::addDatabase("QPSQL");
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::openConnection(const QString& host, const QString& dbName, const QString& user, const QString& password)
{
    m_database.setHostName(host);
    m_database.setDatabaseName(dbName);
    m_database.setUserName(user);
    m_database.setPassword(password);

    if (!m_database.open()) {
        qDebug() << "Ошибка подключения к базе данных:" << m_database.lastError().text();
            return false;
    }
    return true;
}

void DatabaseManager::closeConnection()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

QSqlDatabase DatabaseManager::database() const
{
    return m_database;
}
