#include "naujaseka.h"
#include "ui_naujaseka.h"

#include "settings.h"
#include "databasemanager.h"
#include "klientupaieska.h"

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

<<<<<<< HEAD
    QDate yy = QDate::currentDate();
    yy = yy.addYears(1);
    ui->dateEdit_gar->setDate(yy);
=======
    ui->lineEdit_user->setReadOnly(true);

    client_record = QSqlRecord();
>>>>>>> origin/master

}

NaujasEka::~NaujasEka()
{
    delete ui;
}

void NaujasEka::on_pushButton_save_clicked()
{

    int row = ui->comboBox_model->currentIndex();
    QModelIndex index;
    index = models_model->index(row, 0);

    QVariant eka_client_id = client_record.isEmpty() ? 0 : client_record.value(0);
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
    bool eka_place = ui->checkBox_place->isChecked();

    QSqlRecord record = eka_model->record();

    record.remove(0); //eka_id
    record.remove(12); //status
    record.remove(12); //eka_contract

    int i = 0;
    record.setValue(i++, eka_model_id);
    record.setValue(i++, eka_client_id);

    record.setValue(i++, QVariant(eka_serial_number));
    record.setValue(i++, QVariant(eka_certificate));
    record.setValue(i++, QVariant(eka_count_of_use));
    record.setValue(i++, QVariant(eka_reg_data));
    record.setValue(i++, QVariant(eka_main_checkup));
    record.setValue(i++, QVariant(eka_warranty));

    record.setValue(i++, QVariant(eka_reg_warranty));
    record.setValue(i++, QVariant(eka_rent));
    record.setValue(i++, QVariant(eka_reg_rent));
    record.setValue(i++, QVariant(eka_place_eka));
    record.setValue(i++, QVariant(eka_place));

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

void NaujasEka::setClient()
{
    QSqlRecord client = client_search_dialog->getCurrentClient();
    if (!client.isEmpty())
    {
        client_record = client;
        ui->lineEdit_user->setText(client.value(1).toString());
    }
}

void NaujasEka::on_toolButton_clicked()
{
    client_search_dialog = new KlientuPaieska(this, dbm);
    client_search_dialog->setModal(true);
    connect(client_search_dialog, SIGNAL(accepted()), this, SLOT(setClient()));
    client_search_dialog->exec();
}
