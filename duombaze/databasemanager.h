#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QWidget>

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
    void init();
    void promptSqlErrorMessage(QWidget *parent, const QSqlError &error);
};

#endif // DATABASEMANAGER_H
