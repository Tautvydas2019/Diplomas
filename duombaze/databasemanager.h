#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "qdbfrecord.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QWidget>

using namespace QDbf;

class DatabaseManager
{
protected:
    QSqlDatabase db;
    void createTables();
    void dbError(const QSqlError &);
public:
    DatabaseManager();
    ~DatabaseManager();
    QSqlDatabase getDatabase();
    void createTempTables();
    void importDbfClientRecord(const QDbfRecord &record);
    void importDbfEkaRecord(const QDbfRecord &record);
    void importDbfModelRecord(const QDbfRecord &record);
    void init();
    void promptSqlErrorMessage(QWidget *parent, const QSqlError &error);
};

#endif // DATABASEMANAGER_H
