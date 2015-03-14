#include "naujaseka.h"
#include "ui_naujaseka.h"

#include "settings.h"
#include "databasemanager.h"

#include <QWidget>
#include <QDialog>
#include <QSqlTableModel>
#include <QDate>
#include <QModelIndex>
#include <QVariant>
#include <QString>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>

#include <QDebug>



NaujasEka::NaujasEka(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::NaujasEka)
{
    this->dbm = dbm;
    ui->setupUi(this);
    QWidget::setWindowTitle(Settings::TEXT_NAUJASEKA_NAME);

    models_model = new QSqlTableModel(this, dbm->getDatabase());
    models_model->setTable(Settings::MODEL_TABLE);
    models_model->select();

    ui->comboBox_model->setModel(models_model);
    ui->comboBox_model->setModelColumn(1);

    eka_model = new QSqlTableModel(this, dbm->getDatabase());
    eka_model->setTable(Settings::EKA_TABLE);
    eka_model->select();

    ui->tableView->setModel(eka_model);

    QDate date = QDate::currentDate();
    ui->dateEdit_reg->setDate(date);
    ui->dateEdit_nuom->setDate(date);
    ui->dateEdit_prof->setDate(date);

    ui->dateEdit_nuom->setEnabled(false);

    QDate yy = QDate::currentDate();
    yy = yy.addYears(1);
    ui->dateEdit_gar->setDate(yy);

}

NaujasEka::~NaujasEka()
{
    delete ui;
    delete models_model;
    delete eka_model;
}

void NaujasEka::on_pushButton_save_clicked()
{

    int row = ui->comboBox_model->currentIndex();
    QModelIndex index;
    index = models_model->index(row, 0);
    QVariant eka_model_id = models_model->data(index);

    QString eka_serial_number = ui->lineEdit_ekanr->text();
    QString eka_certificate = ui->lineEdit_cert->text();
    QString eka_count_of_use = ui->lineEdit_number->text();
    QDate eka_reg_data = ui->dateEdit_reg->date();
    QDate eka_main_checkup = ui->dateEdit_prof->date();
    bool eka_warranty = ui->checkBox_gar->isChecked();

    QDate eka_reg_warranty = ui->dateEdit_gar->date();
    bool eka_rent = ui->checkBox_nuom->isChecked();
    QDate eka_reg_rent = ui->dateEdit_nuom->date();
    QString eka_place_eka = ui->lineEdit_ekaplace->text();
    bool eka_place = ui->checkBox_place->isChecked(); // nzn ar sitas
    QString eka_contract = "N"; // nera
    QString eka_c_name = ui->lineEdit_user->text();

    QSqlRecord record = eka_model->record();
    record.remove(0);
    record.setValue(0, eka_model_id);
    record.setValue(1, QVariant(eka_serial_number));
    record.setValue(2, QVariant(eka_certificate));
    record.setValue(3, QVariant(eka_count_of_use));
    record.setValue(4, QVariant(eka_reg_data));
    record.setValue(5, QVariant(eka_main_checkup));
    record.setValue(6, QVariant(eka_warranty));
    record.setValue(7, QVariant(eka_reg_warranty));
    record.setValue(8, QVariant(eka_rent));
    record.setValue(9, QVariant(eka_reg_rent));
    record.setValue(10, QVariant(eka_place_eka));
    record.setValue(12, QVariant(eka_contract));
    record.setValue(13, QVariant(eka_c_name));
    record.setValue(14, QVariant(eka_place));

    bool inserted = eka_model->insertRecord(-1, record);
    if (!inserted)
    {
        dbm->promptSqlErrorMessage(this, eka_model->lastError());
    }
}

void NaujasEka::on_checkBox_nuom_stateChanged(int checked)
{
    if (checked)
    {
        ui->dateEdit_nuom->setEnabled(true);
        ui->dateEdit_gar->setEnabled(false);
    }
    else
    {
        ui->dateEdit_nuom->setEnabled(false);
        ui->dateEdit_gar->setEnabled(true);
    }
}
