#include "importdbf.h"
#include "ui_importdbf.h"

#include "qdbfrecord.h"
#include "qdbftable.h"
#include "settings.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QVariant>
#include <QStringList>
#include <QString>
#include <QSqlTableModel>
#include <QMap>
#include <QSqlQuery>

#include <QDebug>

using namespace QDbf;

ImportDbf::ImportDbf(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::ImportDbf)
{
    this->dbm = dbm;
    ui->setupUi(this);

    this->dbm->createTempTables();
    this->dbm->deleteTempTablesData();

    ui->lineEdit_client->setReadOnly(true);
    ui->lineEdit_eka->setReadOnly(true);
    ui->lineEdit_model->setReadOnly(true);

    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);

    QStringList settings = {"Jokio", "A konvertinimas", "B Konvertinimas"};
    ui->comboBox_client->addItems(settings);
    ui->comboBox_model->addItems(settings);
    ui->comboBox_eka->addItems(settings);

    ui->pushButton_client->setDisabled(true);
    ui->pushButton_model->setDisabled(true);
    ui->pushButton_eka->setDisabled(true);

    client_model = new QSqlTableModel(this, dbm->getDatabase());
    eka_model = new QSqlTableModel(this, dbm->getDatabase());
    model_model = new QSqlTableModel(this, dbm->getDatabase());

    client_model->setTable(Settings::TEMP_CLIENT_TABLE);
    eka_model->setTable(Settings::TEMP_EKA_TABLE);
    model_model->setTable(Settings::TEMP_MODEL_TABLE);
    client_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    eka_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model_model->setEditStrategy(QSqlTableModel::OnFieldChange);

    ui->tableView_eka->setModel(eka_model);
    ui->tableView_model->setModel(model_model);
    ui->tableView_client->setModel(client_model);
    ui->tableView_eka->hideColumn(0);
    ui->tableView_eka->hideColumn(1);
    ui->tableView_eka->hideColumn(2);
    ui->tableView_model->hideColumn(0);
    ui->tableView_client->hideColumn(0);
}

ImportDbf::~ImportDbf()
{
    dbm->deleteTempTablesData();
    delete ui;
}

void ImportDbf::on_toolButton_eka_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Pasirinkti eka .dbf failą");
    if (eka_table.open(file_name, QDbfTable::ReadOnly) && eka_table.next() &&
        (eka_table.record().count() == 20 || eka_table.record().count() == 21 || eka_table.record().count() == 23))
    {
        if (eka_table.record().count() == 21)
        {
            ui->comboBox_eka->setCurrentIndex(0);
        }
        else if (eka_table.record().count() == 20)
        {
            ui->comboBox_eka->setCurrentIndex(1);
        }
        else if (eka_table.record().count() == 23)
        {
            ui->comboBox_eka->setCurrentIndex(2);
        }
        ui->lineEdit_eka->setText(file_name);
        ui->pushButton_eka->setEnabled(true);
    }
    else
    {
        if (file_name.length() > 0)
        {
            QMessageBox::warning(this, "Klaidingas failas!", "Pasirinkote klaidingą .dbf eka failą");
        }
    }
}

void ImportDbf::on_toolButton_client_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Pasirinkti klientų .dbf failą");
    if (client_table.open(file_name, QDbfTable::ReadOnly) && client_table.next() && client_table.record().count() == 7)
    {
        ui->lineEdit_client->setText(file_name);
        ui->pushButton_client->setEnabled(true);
        ui->comboBox_client->setCurrentIndex(1);
    }
    else
    {
        if (file_name.length() > 0)
        {
            QMessageBox::warning(this, "Klaidingas failas!", "Pasirinkote klaidingą .dbf klientų failą");
        }
    }
}

void ImportDbf::on_toolButton_model_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Pasirinkti modelių .dbf failą");
    if (model_table.open(file_name, QDbfTable::ReadOnly) && model_table.next() && model_table.record().count() == 2)
    {
        ui->lineEdit_model->setText(file_name);
        ui->pushButton_model->setEnabled(true);
        ui->comboBox_model->setCurrentIndex(0);
    }
    else
    {
        if (file_name.length() > 0)
        {
            QMessageBox::warning(this, "Klaidingas failas!", "Pasirinkote klaidingą .dbf modelių failą");
        }
    }
}

void ImportDbf::on_pushButton_eka_clicked()
{
    ui->pushButton_eka->setEnabled(false);
    eka_table.open(ui->lineEdit_eka->text());
    int dbf_size = eka_table.size();
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(dbf_size);
    int current_row = 0;
    int fix_enc_type = ui->comboBox_eka->currentIndex();
    dbm->getDatabase().transaction();
    while (eka_table.next())
    {
        QMap<QString, QVariant> data;
        QDbfRecord record = eka_table.record();
        for (int i = 0; i < record.count(); i++)
        {
            data.insert(record.fieldName(i), record.value(i));
        }
        dbm->importDbfEkaRecord(data, fix_enc_type);
        current_row++;
        ui->progressBar->setValue(current_row);
    }
    dbm->getDatabase().commit();
    eka_model->select();
    ui->lineEdit_eka->setText("");
    ui->progressBar->setVisible(false);
}

void ImportDbf::on_pushButton_client_clicked()
{
    ui->pushButton_client->setEnabled(false);
    client_table.open(ui->lineEdit_client->text());
    int dbf_size = client_table.size();
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(dbf_size);
    int current_row = 0;
    int fix_enc_type = ui->comboBox_client->currentIndex();
    dbm->getDatabase().transaction();
    while (client_table.next())
    {
        dbm->importDbfClientRecord(client_table.record(), fix_enc_type);
        current_row++;
        ui->progressBar->setValue(current_row);
    }
    dbm->getDatabase().commit();
    client_model->select();
    ui->lineEdit_client->setText("");
    ui->progressBar->setVisible(false);
}

void ImportDbf::on_pushButton_model_clicked()
{
    ui->pushButton_model->setEnabled(false);
    model_table.open(ui->lineEdit_model->text());
    int dbf_size = model_table.size();
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(dbf_size);
    int current_row = 0;
    int fix_enc_type = ui->comboBox_model->currentIndex();
    dbm->getDatabase().transaction();
    while (model_table.next())
    {
        dbm->importDbfModelRecord(model_table.record(), fix_enc_type);
        current_row++;
        ui->progressBar->setValue(current_row);
    }
    dbm->getDatabase().commit();
    model_model->select();
    ui->lineEdit_model->setText("");
    ui->progressBar->setVisible(false);
}

void ImportDbf::on_pushButton_addToDb_clicked()
{
    QSqlQuery count_query;
    QString count_sql = "SELECT COUNT(*) AS total FROM `" + Settings::TEMP_CLIENT_TABLE + "`;";
    count_query.prepare(count_sql);
    if (!count_query.exec())
    {
        dbm->dbError(count_query.lastError());
    }
    count_query.next();
    int total_clients = count_query.value("total").toInt();

    count_query.clear();
    count_sql = "SELECT COUNT(*) AS total FROM `" + Settings::TEMP_MODEL_TABLE + "`;";
    count_query.prepare(count_sql);
    if (!count_query.exec())
    {
        dbm->dbError(count_query.lastError());
    }
    count_query.next();
    int total_models = count_query.value("total").toInt();

    count_query.clear();
    count_sql = "SELECT COUNT(*) AS total FROM `" + Settings::TEMP_EKA_TABLE + "`;";
    count_query.prepare(count_sql);
    if (!count_query.exec())
    {
        dbm->dbError(count_query.lastError());
    }
    count_query.next();
    int total_ekas = count_query.value("total").toInt();

    int total_data = total_models + total_ekas + total_clients;

    int current_clients = 0;
    int current_models = 0;
    int current_ekas = 0;

    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(total_data);

    int current_row= 0;

    dbm->getDatabase().transaction();
    //klientai
    //qDebug() << "client select";
    QSqlQuery select_query;
    QString select_sql = "SELECT * FROM `" + Settings::TEMP_CLIENT_TABLE + "`;";
    select_query.prepare(select_sql);
    if (!select_query.exec())
    {
        dbm->dbError(select_query.lastError());
    }
    while (select_query.next())
    {
        QVariant temp_client_id = select_query.value("temp_client_id");
        QVariant name = select_query.value("PIRKEJES");
        //qDebug() << "client check";
        QSqlQuery check_query;
        QString check_sql = "SELECT COUNT(*) AS total FROM `" + Settings::CLIENT_TABLE + "` WHERE `name` = ?;";
        check_query.prepare(check_sql);
        check_query.bindValue(0, name);
        if (!check_query.exec())
        {
            dbm->dbError(check_query.lastError());
        }
        check_query.next();
        int total = check_query.value("total").toInt();
        if (total == 0) {
            QVariant code = select_query.value("KODAS");
            QVariant vat = select_query.value("PVMKODAS");
            QVariant address = select_query.value("ADRESAS");
            QVariant telephone = select_query.value("TELEFONAS");
            QVariant additional_info = select_query.value("INFO");
            QVariant city = QVariant("");
            //qDebug() << "client insert";
            QSqlQuery insert_query;
            QString insert_sql = "INSERT INTO `" + Settings::CLIENT_TABLE + "` (`name`, `code`, `vat`, `address`, `telephone`, `additional_info`, `city`)"
                                   " VALUES (?, ?, ?, ?, ?, ?, ?);";
            insert_query.prepare(insert_sql);
            insert_query.bindValue(0, name);
            insert_query.bindValue(1, code);
            insert_query.bindValue(2, vat);
            insert_query.bindValue(3, address);
            insert_query.bindValue(4, telephone);
            insert_query.bindValue(5, additional_info);
            insert_query.bindValue(6, city);
            if (!insert_query.exec())
            {
                dbm->dbError(insert_query.lastError());
            }
            QVariant client_id = insert_query.lastInsertId();
            current_clients++;
            //qDebug() << "client update";
            QSqlQuery update_query;
            QString update_sql = "UPDATE `" + Settings::TEMP_EKA_TABLE + "` SET `temp_client_id` = ? WHERE `P_KODAS` = ?;";
            update_query.prepare(update_sql);
            update_query.bindValue(0, client_id);
            update_query.bindValue(1, code);
            if (!update_query.exec())
            {
                dbm->dbError(update_query.lastError());
            }
        }
        //qDebug() << "client delete";
        QSqlQuery delete_query;
        QString delete_sql = "DELETE FROM `" + Settings::TEMP_CLIENT_TABLE + "` WHERE `temp_client_id` = ?;";
        delete_query.prepare(delete_sql);
        delete_query.bindValue(0, temp_client_id);
        if (!delete_query.exec())
        {
            dbm->dbError(delete_query.lastError());
        }
        current_row++;
        ui->progressBar->setValue(current_row);
    }

    //import modelis
    //qDebug() << "import select";
    QSqlQuery check_query;
    QString check_sql = "SELECT COUNT(*) AS total FROM `" + Settings::MODEL_TABLE + "` WHERE `letters` = ?;";
    check_query.prepare(check_sql);
    check_query.bindValue(0, QVariant("#I"));
    QVariant import_model_id;
    if (!check_query.exec())
    {
        dbm->dbError(check_query.lastError());
    }
    check_query.next();
    int total = check_query.value("total").toInt();
    if (total == 0)
    {
        //qDebug() << "import insert";
        QSqlQuery insert_query;
        QString insert_sql = "INSERT INTO `" + Settings::MODEL_TABLE + "` (`name`, `letters`)"
                             " VALUES (?, ?);";
        insert_query.prepare(insert_sql);
        insert_query.bindValue(0, QVariant("IMPORTAVIMO MODELIS"));
        insert_query.bindValue(1, QVariant("#I"));
        if (!insert_query.exec())
        {
            dbm->dbError(insert_query.lastError());
        }
        import_model_id = insert_query.lastInsertId();
        current_models++;
    }
    else
    {
        select_sql = "SELECT * FROM `" + Settings::MODEL_TABLE + "` WHERE `letters` = ?;";
        select_query.prepare(select_sql);
        select_query.bindValue(0, QVariant("#I"));
        if (!select_query.exec())
        {
            dbm->dbError(select_query.lastError());
        }
        select_query.next();
        import_model_id = select_query.value("model_id");
    }
    //qDebug() << "import update";
    QSqlQuery update_query;
    QString update_sql = "UPDATE `" + Settings::TEMP_EKA_TABLE + "` SET `temp_model_id` = ? WHERE 0=0;";
    update_query.prepare(update_sql);
    update_query.bindValue(0, import_model_id);
    if (!update_query.exec())
    {
        dbm->dbError(update_query.lastError());
    }

    //client import
    //qDebug() << "import client check";
    check_query.clear();
    check_sql = "SELECT COUNT(*) AS total FROM `" + Settings::CLIENT_TABLE + "` WHERE `name` = ?;";
    check_query.prepare(check_sql);
    check_query.bindValue(0, QVariant("NEŽINOMAS KLIENTAS"));
    QVariant import_client_id;
    if (!check_query.exec())
    {
        dbm->dbError(check_query.lastError());
    }
    check_query.next();
    total = check_query.value("total").toInt();
    if (total == 0)
    {
        //qDebug() << "import client insert";
        QSqlQuery insert_query;
        QString insert_sql = "INSERT INTO `" + Settings::CLIENT_TABLE + "` (`name`, `code`, `vat`, `address`, `telephone`, `additional_info`, `city`)"
                             " VALUES (?, ?, ?, ?, ?, ?, ?);";
        insert_query.prepare(insert_sql);
        insert_query.bindValue(0, QVariant("NEŽINOMAS KLIENTAS"));
        insert_query.bindValue(1, QVariant("0000000"));
        insert_query.bindValue(2, QVariant("000000000"));
        insert_query.bindValue(3, QVariant("NEŽINOMAS ADRESAS"));
        insert_query.bindValue(4, QVariant("000000000"));
        insert_query.bindValue(5, QVariant("Šis klientas buvo sukurtas importuotiems EKA kai nebuvo surastas tikrasis klientas"));
        insert_query.bindValue(6, QVariant("NEŽINOMAS MIESTAS"));

        if (!insert_query.exec())
        {
            dbm->dbError(insert_query.lastError());
        }
        import_client_id = insert_query.lastInsertId();
        current_clients++;
    }
    else
    {
        select_sql = "SELECT * FROM `" + Settings::CLIENT_TABLE + "` WHERE `name` = ?;";
        select_query.prepare(select_sql);
        select_query.bindValue(0, QVariant("NEŽINOMAS KLIENTAS"));
        if (!select_query.exec())
        {
            dbm->dbError(select_query.lastError());
        }
        select_query.next();
        import_client_id = select_query.value("client_id");
    }
    //qDebug() << "import client update";
    update_query.clear();
    update_sql = "UPDATE `" + Settings::TEMP_EKA_TABLE + "` SET `temp_client_id` = ? WHERE `temp_client_id` = ? OR `temp_client_id` IS NULL;";
    update_query.prepare(update_sql);
    update_query.bindValue(0, import_client_id);
    update_query.bindValue(1, QVariant(""));
    if (!update_query.exec())
    {
        dbm->dbError(update_query.lastError());
    }

    //eka
    //qDebug() << "eka select";
    select_query.clear();
    select_sql = "SELECT * FROM `" + Settings::TEMP_EKA_TABLE + "`;";
    select_query.prepare(select_sql);
    if (!select_query.exec())
    {
        dbm->dbError(select_query.lastError());
    }
    while (select_query.next())
    {
        QVariant temp_eka_id = select_query.value("temp_eka_id");
        QVariant model_id = select_query.value("temp_model_id");
        QVariant client_id = select_query.value("temp_client_id");
        QVariant serial_number = select_query.value("P_KODAS");
        //qDebug() << "eka check";
        QSqlQuery check_query;
        QString check_sql = "SELECT COUNT(*) AS total FROM `" + Settings::EKA_TABLE + "` WHERE `serial_number` = ?;";
        check_query.prepare(check_sql);
        check_query.bindValue(0, serial_number);
        if (!check_query.exec())
        {
            dbm->dbError(check_query.lastError());
        }
        check_query.next();
        int total = check_query.value("total").toInt();
        if (total == 0)
        {
            QVariant certificate = select_query.value("TPASAS");
            QVariant count_of_use = select_query.value("KIEK");
            QVariant reg_date = select_query.value("REG_DATA");
            QVariant main_checkup = select_query.value("PROF_DATA");
            QVariant reg_warranty = select_query.value("GAR_DATA");
            QVariant reg_rent = select_query.value("NUO_DATA");
            QVariant place_eka = select_query.value("USE");
            QVariant status = select_query.value("DONT").toBool() ? QVariant("Nedirba") : QVariant("Dirba");
            QVariant contract = QVariant("");
            QVariant place = select_query.value("VILLAGE").toBool() ? QVariant(false) : QVariant(true);
            //qDebug() << "eka insert";
            QSqlQuery insert_query;
            QString insert_sql = "INSERT INTO `" + Settings::EKA_TABLE + "` (`model_id`, `client_id`, `serial_number`, `certificate`, `count_of_use`, `reg_data`, `main_checkup`, `reg_warranty`, `reg_rent`, `place_eka`, `status`, `contract`, `place`)"
                                   " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
            insert_query.prepare(insert_sql);
            insert_query.bindValue(0, model_id);
            insert_query.bindValue(1, client_id);
            insert_query.bindValue(2, serial_number);
            insert_query.bindValue(3, certificate);
            insert_query.bindValue(4, count_of_use);
            insert_query.bindValue(5, reg_date);
            insert_query.bindValue(6, main_checkup);
            insert_query.bindValue(7, reg_warranty);
            insert_query.bindValue(8, reg_rent);
            insert_query.bindValue(9, place_eka);
            insert_query.bindValue(10, status);
            insert_query.bindValue(11, contract);
            insert_query.bindValue(12, place);
            if (!insert_query.exec())
            {
                dbm->dbError(insert_query.lastError());
            }
            current_ekas++;
        }
        //qDebug() << "eka delete";
        QSqlQuery delete_query;
        QString delete_sql = "DELETE FROM `" + Settings::TEMP_EKA_TABLE + "` WHERE `temp_eka_id` = ?;";
        delete_query.prepare(delete_sql);
        delete_query.bindValue(0, temp_eka_id);
        if (!delete_query.exec())
        {
            dbm->dbError(delete_query.lastError());
        }
        current_row++;
        ui->progressBar->setValue(current_row);
    }

    //model
    //qDebug() << "model select";
    select_query.clear();
    select_sql = "SELECT * FROM `" + Settings::TEMP_MODEL_TABLE + "`;";
    select_query.prepare(select_sql);
    if (!select_query.exec())
    {
        dbm->dbError(select_query.lastError());
    }
    while (select_query.next())
    {
        QVariant temp_model_id = select_query.value("temp_model_id");
        QVariant name = select_query.value("MODEL");
        QVariant letters = select_query.value("LETTER");

        //qDebug() << "model check";
        QSqlQuery check_query;
        QString check_sql = "SELECT COUNT(*) AS total FROM `" + Settings::MODEL_TABLE + "` WHERE `name` = ? OR `letters` = ?;";
        check_query.prepare(check_sql);
        check_query.bindValue(0, name);
        check_query.bindValue(1, letters);
        if (!check_query.exec())
        {
            dbm->dbError(check_query.lastError());
        }
        check_query.next();
        int total = check_query.value("total").toInt();
        if (total == 0)
        {
            //qDebug() << "model insert";
            QSqlQuery insert_query;
            QString insert_sql = "INSERT INTO `" + Settings::MODEL_TABLE + "` (`name`, `letters`)"
                                   " VALUES (?, ?);";
            insert_query.prepare(insert_sql);
            insert_query.bindValue(0, name);
            insert_query.bindValue(1, letters);
            if (!insert_query.exec())
            {
                dbm->dbError(insert_query.lastError());
            }
            current_models++;
        }
        //qDebug() << "model delete";
        QSqlQuery delete_query;
        QString delete_sql = "DELETE FROM `" + Settings::TEMP_MODEL_TABLE + "` WHERE `temp_model_id` = ?;";
        delete_query.prepare(delete_sql);
        delete_query.bindValue(0, temp_model_id);
        if (!delete_query.exec())
        {
            dbm->dbError(delete_query.lastError());
        }
        current_row++;
        ui->progressBar->setValue(current_row);
    }
    dbm->getDatabase().commit();
    model_model->select();
    eka_model->select();
    client_model->select();
    ui->progressBar->setVisible(false);
    QString title = "Baigtas Importavimas";
    QString message = "<h2>Importavimas baigtas</h2><strong>Importuota:</strong><ul><li>" +
            QString::number(current_clients) + " iš " + QString::number(total_clients) + " klientų.</li><li>" +
            QString::number(current_ekas) + " iš " + QString::number(total_ekas) + " EKA.</li><li>" +
            QString::number(current_models) + " iš " + QString::number(total_models) + " modelių.</li></ul>";
    QMessageBox::information(this, title, message);
}



void ImportDbf::on_pushButton_deleteEka_clicked()
{
    dbm->deleteTempEkaTableData();
    eka_model->select();
}

void ImportDbf::on_pushButton_deleteClient_clicked()
{
    dbm->deleteTempClientTableData();
    client_model->select();
}

void ImportDbf::on_pushButton_deleteModel_clicked()
{
    dbm->deleteTempModelTableData();
    model_model->select();
}
