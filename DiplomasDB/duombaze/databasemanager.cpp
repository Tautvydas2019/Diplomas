#include "databasemanager.h"

#include "settings.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QMap>

DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase(Settings::DB_TYPE);
}

DatabaseManager::~DatabaseManager()
{
    db.close();
}

void DatabaseManager::init()
{
    db.setDatabaseName(Settings::DB_LOCATION);
    if (!db.open())
    {
        dbError(db.lastError());
    }
    createTables();
}

void DatabaseManager::createTables()
{
    QSqlQuery query;
    for (auto &table : Settings::DB_TABLES)
    {
        query.prepare(table);
        if (!query.exec())
        {
            dbError(query.lastError());
        }
        query.clear();
    }
}

void DatabaseManager::dbError(const QSqlError &error)
{
    qDebug() << "text:" << error.text();
    qDebug() << "type:" << error.type();
    qFatal(error.text().toStdString().c_str());
}

QMap<QString, QString> DatabaseManager::genResultMap(const QSqlQuery &query)
{
    QSqlRecord record = query.record();
    int columns = record.count();
    QMap<QString, QString> results;
    for (int c = 0; c < columns; c++)
    {
        results[record.fieldName(c)] = query.value(c).toString();
    }
    return results;
}

//QMap<QString, QMap<QString, QString>> DatabaseManager::getClients()
//{
//    QString sql = "SELECT * FROM `" + Settings::DB_TABLE_CLIENT + "` ORDER BY `date_added` ASC;";
//    QSqlQuery query;
//    query.prepare(sql);
//    if (!query.exec())
//    {
//        dbError(query.lastError());
//    }
//    QMap<QString, QMap<QString, QString>> results;
//    for (int r = 0; query.next(); r++) {
//        results[QString::number(r)] = genResultMap(query);
//    }
//    query.clear();
//    return results;
//}

QMap<QString, QMap<QString, QString>> DatabaseManager::getModels()
{
    QString sql = "SELECT * FROM `" + Settings::MODEL_TABLE + "`;";
    QSqlQuery query;
    query.prepare(sql);
    if (!query.exec())
    {
        dbError(query.lastError());
    }
    QMap<QString, QMap<QString, QString>> results;
    for (int r = 0; query.next(); r++) {
        results[QString::number(r)] = genResultMap(query);
    }
    query.clear();
    return results;
}

void DatabaseManager::insertModel(const QString &model_name, const QString &model_letters)
{
    QString sql = "INSERT INTO `" + Settings::MODEL_TABLE + "` (`name`, `letters`) VALUES (:name, :letters);";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":name", model_name);
    query.bindValue(":letters", model_letters);
    if (!query.exec())
    {
        dbError(query.lastError());
    }
    query.clear();
}

//void DatabaseManager::deleteClient(const int &client_id) {
//    QString sql = "DELETE FROM `" + Settings::DB_TABLE_CLIENT + "` WHERE `client_id` = ?;";
//    QSqlQuery query;
//    query.prepare(sql);
//    query.addBindValue(client_id);
//    if (!query.exec())
//    {
//        dbError(query.lastError());
//    }
//    query.clear();
//}

//void DatabaseManager::deleteClient(const QString &client_id) {
//    QString sql = "DELETE FROM `" + Settings::DB_TABLE_CLIENT + "` WHERE `client_id` = ?;";
//    QSqlQuery query;
//    query.prepare(sql);
//    query.addBindValue(client_id);
//    if (!query.exec())
//    {
//        dbError(query.lastError());
//    }
//    query.clear();
//}
