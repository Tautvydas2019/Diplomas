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
    QString sql = "INSERT INTO `" + Settings::CLIENT_TABLE + "` (`name`, `code`, `vat`, `address`, `telephone`, `additional_info`) "
                                                             "VALUES (:name, :code, :vat, :address, :telephone, :additional_info);";
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

QMap<QString, QMap<QString, QString>> DatabaseManager::getEka()
{
    QString sql = "SELECT * FROM `" + Settings::EKA_TABLE + "`;";
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

void DatabaseManager::insertEka(const QString &eka_serial_number, const QString &eka_certificate, const QString &eka_count_of_use,
                                const QString &eka_reg_data, const QString &eka_main_checkup, const QString &eka_warranty,
                                const QString &eka_rent, const QString &eka_reg_rent, const QString &eka_place_eka,
                                const QString &eka_eka_model, const QString &eka_contract, const QString &eka_c_name)
{

    QString sql = "INSERT INTO `" + Settings::EKA_TABLE + "` (`serial_number`, `certificate`, `count_of_use`, `reg_data`, `main_checkup`,"
                                                          "`warranty`, `rent`, `reg_rent`, `place_eka`, `eka_model`, eka_contract, `c_name`"
                                                          "VALUES (:serial_number, :certificate, :count_of_use, :reg_data, :main_checkup,"
                                                          ":warranty, :rent, :reg_rent, :place_eka, :eka_model, :eka_contract, :c_name);";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":serial_number", eka_serial_number);
    query.bindValue(":certificate", eka_certificate);
    query.bindValue(":count_of_use", eka_count_of_use);
    query.bindValue(":reg_data", eka_reg_data);
    query.bindValue(":main_checkup", eka_main_checkup);
    query.bindValue(":warranty", eka_warranty);
    query.bindValue(":rent", eka_rent);
    query.bindValue(":reg_rent", eka_reg_rent);
    query.bindValue(":place_eka", eka_place_eka);
    query.bindValue(":eka_model", eka_eka_model);
    query.bindValue(":eka_contract", eka_contract);
    query.bindValue(":c_name", eka_c_name);
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
