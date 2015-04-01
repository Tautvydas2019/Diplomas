#include "settings.h"

#include <QString>
#include <QList>

const QString Settings::TEXT_MAINWINDOW_NAME =
        "Pagrindinis";

const QString Settings::TEXT_NAUJASKLIENTAS_NAME =
        "Naujas klientas";

const QString Settings::TEXT_NAUJASMODELIS_NAME =
        "Naujas modelis";

const QString Settings::TEXT_NAUJASEKA_NAME =
        "Naujas EKA";

const QString Settings::TEXT_KLIENTUPAIESKA_NAME =
        "Klientų duomenų bazė";

const QString Settings::DB_TYPE =
        "QSQLITE";

const QString Settings::DB_LOCATION =
        "./EKA_client.db";

const QList<QString> Settings::DB_TABLES = {

    "CREATE TABLE IF NOT EXISTS `model` ("
      "`model_id` INTEGER PRIMARY KEY AUTOINCREMENT,"
      "`name` VARCHAR(20) NOT NULL UNIQUE,"
      "`letters` VARCHAR(2) NOT NULL UNIQUE"

     ");",

    "CREATE TABLE IF NOT EXISTS `client` ("
      "`client_id` INTEGER PRIMARY KEY AUTOINCREMENT,"
      "`name` VARCHAR(100) NOT NULL UNIQUE,"
      "`code` INT(10),"
      "`vat` VARCHAR(20),"
      "`address` VARCHAR(100),"
      "`telephone` VARCHAR(15),"
      "`additional_info` VARCHAR(255),"
      "`city` VARCHAR(20)"
     ");",

    "CREATE TABLE IF NOT EXISTS `eka` ("
      "`eka_id` INTEGER PRIMARY KEY AUTOINCREMENT, "
      "`model_id` INTEGER,"
      "`client_id` INTEGER NOT NULL,"
      "`serial_number` INTEGER(8)NOT NULL UNIQUE,"
      "`certificate` VARCHAR(20),"
      "`count_of_use` INTEGER(1),"
      "`reg_data` DATE,"
      "`main_checkup` DATE,"
      "`warranty` BOOLEAN,"
      "`reg_warranty` DATE,"
      "`rent` BOOLEAN,"
      "`reg_rent` DATE,"
      "`place_eka` VARCHAR(100),"
      "`status` BOOLEAN,"
      "`contract` VARCHAR(1),"
      "`place` BOOLEAN"
    ");"
};

const QString Settings::CLIENT_TABLE =
        "client";

const QString Settings::MODEL_TABLE =
        "model";

const QString Settings::EKA_TABLE =
        "eka";

