#include "databasemanager.h"

#include "settings.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>
#include <QString>
#include <QMessageBox>

#include <QDebug>


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
    qFatal(error.text().toStdString().c_str());
}

//Sukuria žinute apie sql klaidą
void DatabaseManager::promptSqlErrorMessage(QWidget *parent, const QSqlError &error)
{
    QString title = "Duomenų bazės klaida!";
    QString message = "Įvyko duomenų bazės klaida: " + error.text();
    QMessageBox::critical(parent, title, message);
}
