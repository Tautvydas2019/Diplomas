#include "databasemanager.h"
#include "settings.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QMap>

//Prideda duomenų bazę
DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase(Settings::DB_TYPE);
}

//Uždaro duomenų bazę
DatabaseManager::~DatabaseManager()
{
    db.close();
}

//Sukuria/paima duomenų bazės lenteles
void DatabaseManager::init()
{
    db.setDatabaseName(Settings::DB_LOCATION);
    if (!db.open())
    {
        dbError(db.lastError());
    }
    createTables();
}

//Gražina duomenų bazę
QSqlDatabase DatabaseManager::getDatabase()
{
    return db;
}

//Sukuria programoje duomenų bazė programoje
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

//Parašo duomenų bazės klaidą
void DatabaseManager::dbError(const QSqlError &error)
{
    qDebug() << "text:" << error.text();
    qDebug() << "type:" << error.type();
    qFatal(error.text().toStdString().c_str());
}

//Iš gautų duomenų sukuria QMap
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

//Išrenka duomenis iš duomenų bazės modelio lentelės
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

//Įrašo informacija į duomenų bazės modelio lentelę
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

QMap<QString, QMap<QString, QString>> DatabaseManager::getClient()
{
    QString sql = "SELECT * FROM `" + Settings::CLIENT_TABLE + "`;";
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

void DatabaseManager::insertClient(const QString &client_name, const QString &client_code, const QString &client_vat,
                                   const QString &client_address, const QString &client_telephone, const QString &client_additional_info)
{
    QString sql = "INSERT INTO `" + Settings::CLIENT_TABLE + "` (`name`, `code`, `vat`, `address`, `telephone`, `additional_info`) VALUES (:name, :code, :vat, :address, :telephone, :additional_info);";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":name", client_name);
    query.bindValue(":code", client_code);
    query.bindValue(":vat", client_vat);
    query.bindValue(":address", client_address);
    query.bindValue(":telephone", client_telephone);
    query.bindValue(":additional_info", client_additional_info);
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
