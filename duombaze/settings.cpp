#include "settings.h"

#include <QString>
#include <QList>

const QString Settings::TEXT_MAINWINDOW_NAME =
        "Pagrindinis";
const QString Settings::TEXT_NAUJASKLIENTAS_NAME =
        "Naujas klientas";
const QString Settings::TEXT_NAUJASMODELIS_NAME =
        "Naujas modelis";

const QString Settings::DB_TYPE =
        "QSQLITE";

const QString Settings::DB_LOCATION =
        "./EKA_client.db";

const QList<QString> Settings::DB_TABLES = {

    "CREATE TABLE IF NOT EXISTS `model` ("
      "`model_id` INTEGER PRIMARY KEY AUTOINCREMENT,"
      "`name` VARCHAR(20),"
      "`letters` VARCHAR(2)"
     ");",

    "CREATE TABLE IF NOT EXISTS `client` ("
      "`client_id` INTEGER PRIMARY KEY AUTOINCREMENT,"
      "`name` VARCHAR(100) NOT NULL,"
      "`code` INT(7),"
      "`vat` VARCHAR(20),"
      "`address` VARCHAR(100) NOT NULL,"
      "`telephone` VARCHAR(15),"
      "`additional_info` VARCHAR(255)"
     ");"
};

const QString Settings::CLIENT_TABLE = "client";
const QString Settings::MODEL_TABLE = "model";

