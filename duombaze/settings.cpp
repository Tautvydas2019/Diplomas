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

const QString Settings::TEXT_EKAEDITING_NAME =
        "EKA duomenų bazė";

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
      "`reg_warranty` DATE,"
      "`reg_rent` DATE,"
      "`place_eka` VARCHAR(100),"
      "`status` BOOLEAN DEFAULT Dirba,"
      "`contract` VARCHAR(1),"
      "`place` BOOLEAN"
    ");"
};

const QList<QString> Settings::TEMP_DB_TABLES = {

    "CREATE TABLE IF NOT EXISTS `temp_model` ("
      "`temp_model_id` INTEGER PRIMARY KEY AUTOINCREMENT,"
      "`MODEL` TEXT,"
      "`LETTER` TEXT"
    ");",

    "CREATE TABLE IF NOT EXISTS `temp_client` ("
      "`temp_client_id` INTEGER PRIMARY KEY AUTOINCREMENT,"
      "`PIRKEJES` TEXT,"
      "`KODAS` INTEGER,"
      "`PVMKODAS` INTEGER,"
      "`ADRESAS` TEXT,"
      "`TELEFONAS` TEXT,"
      "`INFO` TEXT,"
      "`STATUS` NUMERIC"
     ");",

    "CREATE TABLE IF NOT EXISTS `temp_eka` ("
      "`temp_eka_id` INTEGER PRIMARY KEY AUTOINCREMENT, "
      "`SPEC` INTEGER,"
      "`KIEK` INTEGER,"
      "`REG_DATA` NUMERIC,"
      "`GAR_DATA` NUMERIC,"
      "`GAR_TYPE` NUMERIC,"
      "`PROF_DATA` NUMERIC,"
      "`KASA` INTEGER,"
      "`TPASAS` TEXT,"
      "`IB_IA` INTEGER,"
      "`P_USER` TEXT,"
      "`P_KODAS` INTEGER,"
      "`USE` TEXT,"
      "`VILLAGE` NUMERIC,"
      "`DELKO` NUMERIC,"
      "`KADA` NUMERIC,"
      "`NUOMA` NUMERIC,"
      "`NUO_DATA` NUMERIC,"
      "`SF` NUMERIC,"
      "`SUMA` INTEGER,"
      "`SUMA_YRA` INTEGER,"
      "`IKI` NUMERIC,"
      "`DONT` NUMERIC,"
      "`DONT_DATA` NUMERIC"
    ");"
};


const QString Settings::CLIENT_TABLE =
        "client";

const QString Settings::MODEL_TABLE =
        "model";

const QString Settings::EKA_TABLE =
        "eka";

const QString Settings::TEMP_CLIENT_TABLE =
        "temp_client";

const QString Settings::TEMP_MODEL_TABLE =
        "temp_model";

const QString Settings::TEMP_EKA_TABLE =
        "temp_eka";

