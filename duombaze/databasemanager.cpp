#include "databasemanager.h"

#include "settings.h"
#include "qdbfrecord.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>
#include <QString>
#include <QVariant>
#include <QMap>
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

void DatabaseManager::deleteTempTablesData()
{
    db.transaction();
    deleteTempClientTableData();
    deleteTempEkaTableData();
    deleteTempModelTableData();
    db.commit();
}

void DatabaseManager::deleteTempClientTableData()
{
    QSqlQuery query;
    query.prepare("DELETE FROM " + Settings::TEMP_CLIENT_TABLE + ";");
    if (!query.exec())
    {
        dbError(query.lastError());
    }
}

void DatabaseManager::deleteTempEkaTableData()
{
    QSqlQuery query;
    query.prepare("DELETE FROM " + Settings::TEMP_EKA_TABLE + ";");
    if (!query.exec())
    {
        dbError(query.lastError());
    }
}

void DatabaseManager::deleteTempModelTableData()
{
    QSqlQuery query;
    query.prepare("DELETE FROM " + Settings::TEMP_MODEL_TABLE + ";");
    if (!query.exec())
    {
        dbError(query.lastError());
    }
}

QVariant DatabaseManager::fixEncoding(const QVariant &value, int type)
{
    if (type == 1)
    {
        QString s_value = value.toString();
        s_value.replace("Ū", "Ė");
        s_value.replace("ė", "Ū");
        s_value.replace("¹", "ą");
        s_value.replace("ź", "ę");
        s_value.replace("š", "ė");
        s_value.replace("ģ", "į");
        s_value.replace("ģ", "į");
        s_value.replace("", "š");
        s_value.replace("ż", "ų");
        s_value.replace("ž", "ū");
        s_value.replace("˛", "ž");
        s_value.replace("�", "Ą");
        s_value.replace("Ź", "Ę");
        s_value.replace("Õ", "Į");
        s_value.replace("", "Š");
        s_value.replace("ē", "Ų");
        s_value.replace("ˇ", "Ž");
        return QVariant(s_value);
    }
    else if (type == 2)
    {
        QString s_value = value.toString();
        s_value.replace("ō", "ų");
        s_value.replace("", "š");
        s_value.replace("ć", "ę");
        s_value.replace("ŗ", "Ą");
        s_value.replace("Ē", "Č");
        s_value.replace("Ź", "Ė");
        s_value.replace("æ", "Į");
        s_value.replace("", "Š");
        s_value.replace("Ņ", "Ų");
        s_value.replace("Ś", "Ū");
        s_value.replace("Ż", "Ž");
        return QVariant(s_value);
    }
    else
    {
        return value;
    }
}

void DatabaseManager::importDbfClientRecord(const QDbfRecord &record, int fix_enc_type)
{
    QSqlQuery query;
    QString sql = "INSERT INTO `" + Settings::TEMP_CLIENT_TABLE + "` (`PIRKEJES`, `KODAS`, `PVMKODAS`, `ADRESAS`, `TELEFONAS`, `INFO`, `STATUS`) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?);";

    query.prepare(sql);
    for (int i = 0; i < record.count(); i++)
    {
        QVariant value = record.value(i);
        value = fixEncoding(value, fix_enc_type);
        query.bindValue(i, value);
    }
    if (!query.exec())
    {
        dbError(query.lastError());
    }
    query.clear();
}

void DatabaseManager::importDbfModelRecord(const QDbfRecord &record, int fix_enc_type)
{
    QSqlQuery query;
    QString sql = "INSERT INTO `" + Settings::TEMP_MODEL_TABLE + "` (`MODEL`, `LETTER`) "
                  "VALUES (?, ?);";

    query.prepare(sql);
    for (int i = 0; i < record.count(); i++)
    {
        QVariant value = record.value(i);
        value = fixEncoding(value, fix_enc_type);
        query.bindValue(i, value);
    }
    if (!query.exec())
    {
        dbError(query.lastError());
    }
    query.clear();
}

void DatabaseManager::importDbfEkaRecord(QMap<QString, QVariant> &record, int fix_enc_type)
{
    QSqlQuery query;
    QString sql = "INSERT INTO `" + Settings::TEMP_EKA_TABLE + "` (`SPEC`, `KIEK`, `REG_DATA`, `GAR_DATA`, `GAR_TYPE`, "
                  "`PROF_DATA`, `KASA`, `TPASAS`, `IB_IA`, `P_USER`, "
                  "`P_KODAS`, `USE`, `VILLAGE`, `DELKO`, `KADA`, "
                  "`NUOMA`, `NUO_DATA`, `SF`, `SUMA`, `SUMA_YRA`, "
                  "`IKI`, `DONT`, `DONT_DATA`) "
                  "VALUES (?, ?, ?, ?, ?, "
                  "?, ?, ?, ?, ?, "
                  "?, ?, ?, ?, ?, "
                  "?, ?, ?, ?, ?, "
                  "?, ?, ?);";

    query.prepare(sql);

    for (QString &key : record.keys())
    {
        QVariant value = record.value(key);
        value = fixEncoding(value, fix_enc_type);
        record.insert(key, value);
    }

    query.bindValue(0, record.value("SPEC"));
    query.bindValue(1, record.value("KIEK"));
    query.bindValue(2, record.value("REG_DATA"));
    query.bindValue(3, record.value("GAR_DATA"));
    query.bindValue(4, record.value("GAR_TYPE"));

    query.bindValue(5, record.value("PROF_DATA"));
    query.bindValue(6, record.value("KASA"));
    query.bindValue(7, record.value("TPASAS"));
    query.bindValue(8, record.value("IB_IA"));
    query.bindValue(9, record.value("P_USER"));

    query.bindValue(10, record.value("P_KODAS"));
    query.bindValue(11, record.value("USE"));
    query.bindValue(12, record.value("VILLAGE"));
    query.bindValue(13, (record.contains("DELKO") ? record.value("DELKO") : QVariant()));
    query.bindValue(14, (record.contains("KADA") ? record.value("KADA") : QVariant()));

    query.bindValue(15, record.value("NUOMA"));
    query.bindValue(16, record.value("NUO_DATA"));
    query.bindValue(17, record.value("SF"));
    query.bindValue(18, record.value("SUMA"));
    query.bindValue(19, record.value("SUMA_YRA"));

    query.bindValue(20, (record.contains("IKI") ? record.value("IKI") : QVariant()));
    query.bindValue(21, record.value("DONT"));
    query.bindValue(22, record.value("DONT_DATA"));

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
