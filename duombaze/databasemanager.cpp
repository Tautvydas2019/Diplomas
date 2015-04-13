#include "databasemanager.h"

#include "settings.h"
#include "qdbfrecord.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>
#include <QString>
#include <QMessageBox>

#include <QDebug>

using namespace QDbf;

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

//Initializuoja duomenų bazę
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

//Sukuria duomenų bazės lenteles
void DatabaseManager::createTables()
{
    db.transaction();
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
    db.commit();
}

void DatabaseManager::createTempTables()
{
    db.transaction();
    QSqlQuery query;
    for (auto &table : Settings::TEMP_DB_TABLES)
    {
        query.prepare(table);
        if (!query.exec())
        {
            dbError(query.lastError());
        }
        query.clear();
    }
    db.commit();
}

void DatabaseManager::importDbfClientRecord(const QDbfRecord &record)
{
    QSqlQuery query;
    QString sql = "INSERT INTO `" + Settings::TEMP_CLIENT_TABLE + "` (`PIRKEJES`, `KODAS`, `PVMKODAS`, `ADRESAS`, `TELEFONAS`, `INFO`, `STATUS`) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?);";

    query.prepare(sql);
    query.bindValue(0, record.value(0));
    query.bindValue(1, record.value(1));
    query.bindValue(2, record.value(2));
    query.bindValue(3, record.value(3));
    query.bindValue(4, record.value(4));
    query.bindValue(5, record.value(5));

    query.bindValue(6, record.value(6));

    if (!query.exec())
    {
        dbError(query.lastError());
    }
    query.clear();
}

void DatabaseManager::importDbfModelRecord(const QDbfRecord &record)
{
    QSqlQuery query;
    QString sql = "INSERT INTO `" + Settings::TEMP_MODEL_TABLE + "` (`MODEL`, `LETTER`) "
                  "VALUES (?, ?);";

    query.prepare(sql);
    query.bindValue(0, record.value(0));
    query.bindValue(1, record.value(1));

    if (!query.exec())
    {
        dbError(query.lastError());
    }
    query.clear();
}

void DatabaseManager::importDbfEkaRecord(const QDbfRecord &record)
{
    QSqlQuery query;
    QString sql = "INSERT INTO `" + Settings::TEMP_EKA_TABLE + "` (`SPEC`, `KIEK`, `REG_DATA`, `GAR_DATA`, `GAR_TYPE`, "
                  "`PROF_DATA`, `KASA`, `TPASAS`, `IB_IA`, `P_USER`, "
                  "`P_KODAS`, `USE`, `VILLAGE`, `NUOMA`, `NUO_DATA`, "
                  "`SF`, `SUMA`, `SUMA_YRA`, `IKI`, `DONT`, "
                  "`DONT_DATA`) "
                  "VALUES (?, ?, ?, ?, ?, "
                  "?, ?, ?, ?, ?, "
                  "?, ?, ?, ?, ?, "
                  "?, ?, ?, ?, ?, "
                  "?);";

    query.prepare(sql);
    query.bindValue(0, record.value(0));
    query.bindValue(1, record.value(1));
    query.bindValue(2, record.value(2));
    query.bindValue(3, record.value(3));
    query.bindValue(4, record.value(4));

    query.bindValue(5, record.value(5));
    query.bindValue(6, record.value(6));
    query.bindValue(7, record.value(7));
    query.bindValue(8, record.value(8));
    query.bindValue(9, record.value(9));

    query.bindValue(10, record.value(10));
    query.bindValue(11, record.value(11));
    query.bindValue(12, record.value(12));
    query.bindValue(13, record.value(13));
    query.bindValue(14, record.value(14));

    query.bindValue(15, record.value(15));
    query.bindValue(16, record.value(16));
    query.bindValue(17, record.value(17));
    query.bindValue(18, record.value(18));
    query.bindValue(19, record.value(19));

    query.bindValue(20, record.value(20));

    if (!query.exec())
    {
        dbError(query.lastError());
    }
    query.clear();
}

//Parašo duomenų bazės klaidą
void DatabaseManager::dbError(const QSqlError &error)
{
    qFatal(error.text().toStdString().c_str());
}

//Sukuria žinute apie sql klaidą
void DatabaseManager::promptSqlErrorMessage(QWidget *parent, const QSqlError &error)
{
    QString title = "Duomenų bazės klaida!";
    QString message = "Įvyko duomenų bazės klaida: " + error.text();
    QMessageBox::critical(parent, title, message);
}
