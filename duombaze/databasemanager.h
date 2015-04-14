#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "qdbfrecord.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QWidget>
#include <QString>
#include <QVariant>
#include <QMap>

using namespace QDbf;

class DatabaseManager
{
protected:
    QSqlDatabase db;
    void createTables();
    void dbError(const QSqlError &);
    QVariant fixEncoding(const QVariant &value, int type);
public:
    DatabaseManager();
    ~DatabaseManager();
    QSqlDatabase getDatabase();
    void createTempTables();
    void deleteTempTablesData();
    void deleteTempClientTableData();
    void deleteTempEkaTableData();
    void deleteTempModelTableData();
    void importDbfClientRecord(const QDbfRecord &record, int fix_enc_type = 0);
    void importDbfEkaRecord(QMap<QString, QVariant> &record, int fix_enc_type = 0);
    void importDbfModelRecord(const QDbfRecord &record, int fix_enc_type = 0);
    void init();
    void promptSqlErrorMessage(QWidget *parent, const QSqlError &error);
};

#endif // DATABASEMANAGER_H
