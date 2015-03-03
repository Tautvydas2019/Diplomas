#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QMap>

class DatabaseManager
{
protected:
    QSqlDatabase db;
    void createTables();
    void dbError(const QSqlError &);
    QMap<QString, QString> genResultMap(const QSqlQuery &);
public:
    DatabaseManager();
    ~DatabaseManager();
    void init();
    QMap<QString, QMap<QString, QString>> getModels();
    void insertModel(const QString &model_name, const QString &model_letters);
//    void deleteClient(const int &);
//    void deleteClient(const QString &);
};

#endif // DATABASEMANAGER_H
