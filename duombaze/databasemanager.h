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
    QSqlDatabase getDatabase();
    void init();
    QMap<QString, QMap<QString, QString>> getModels();
    void insertModel(const QString &model_name, const QString &model_letters);
    QMap<QString, QMap<QString, QString>> getClient();
    void insertClient(const QString &client_name, const QString &client_code, const QString &client_vat,
                      const QString &client_address, const QString &client_telephone, const QString &client_additional_info,
                      const QString &client_city);
    QMap<QString, QMap<QString, QString>> getEka();
    void insertEka(const QString &eka_serial_number, const QString &eka_certificate, const QString &eka_count_of_use,
                      const QString &eka_reg_data, const QString &eka_main_checkup, const QString &eka_warranty,
                      const QString &eka_rent, const QString &eka_reg_rent, const QString &eka_place_eka,
                      const QString &eka_eka_model, const QString &eka_contract, const QString &eka_c_name);
//    void deleteClient(const int &);
//    void deleteClient(const QString &);
};

#endif // DATABASEMANAGER_H
