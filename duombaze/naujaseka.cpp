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
#include <QVariant>

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
    models_model->setSort(2, Qt::AscendingOrder);
    models_model->select();

    ui->comboBox_model->setModel(models_model);
    ui->comboBox_model->setModelColumn(1);

    eka_model = new QSqlTableModel(this, dbm->getDatabase());
    eka_model->setTable(Settings::EKA_TABLE);
    eka_model->select();

    ui->tableView->setModel(eka_model);
    ui->tableView->hide();

    QDate date = QDate::currentDate();
    ui->dateEdit_reg->setDate(date);
    ui->dateEdit_nuom->setDate(date);

    QDate yy = QDate::currentDate();
    yy = yy.addYears(1);
    ui->dateEdit_gar->setDate(yy);
    ui->dateEdit_prof->setDate(yy);

    ui->lineEdit_user->setReadOnly(true);
    ui->dateEdit_nuom->setEnabled(false);
    ui->dateEdit_gar->setEnabled(false);

    ui->dateEdit_nuom->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit_gar->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit_prof->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit_reg->setDisplayFormat("yyyy-MM-dd");

    client_record = QSqlRecord();
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

    QVariant eka_client_id = client_record.isEmpty() ? QVariant() : client_record.value(0);
    QVariant eka_model_id = models_model->data(index);
    QString eka_serial_number = ui->lineEdit_ekanr->text();
    QString eka_certificate = ui->lineEdit_cert->text();
    QString eka_count_of_use = ui->lineEdit_number->text();

    QDate eka_reg_data = ui->dateEdit_reg->date();
    QDate eka_main_checkup = ui->dateEdit_prof->date();

    bool eka_warranty = ui->checkBox_gar->isChecked();
    QString eka_reg_warranty = eka_warranty ? ui->dateEdit_gar->date().toString("yyyy-MM-dd") : "";

    bool eka_rent = ui->checkBox_nuom->isChecked();
    QString eka_reg_rent = eka_rent ? ui->dateEdit_nuom->date().toString("yyyy-MM-dd") : "";

    QString eka_place_eka = ui->lineEdit_ekaplace->text();

    bool eka_place = ui->checkBox_place->isChecked();

    if (eka_serial_number.isEmpty() || eka_place_eka.isEmpty())
    {
        QString title = "Neįvesti duomenys";
        QString message = "Turite įvesti EKA korpuso numerį ir stovėjimo vietą!";
        QMessageBox::warning(this, title, message);
    }
    else
    {
        QSqlRecord record = eka_model->record();
        record.remove(0); //eka_id
        record.remove(10); //status
        record.remove(10); //eka_contract

        int i = 0;
        record.setValue(i++, eka_model_id);
        record.setValue(i++, eka_client_id);

        record.setValue(i++, QVariant(eka_serial_number));
        record.setValue(i++, QVariant(eka_certificate));
        record.setValue(i++, QVariant(eka_count_of_use));
        record.setValue(i++, QVariant(eka_reg_data));
        record.setValue(i++, QVariant(eka_main_checkup));

        record.setValue(i++, QVariant(eka_reg_warranty));

        record.setValue(i++, QVariant(eka_reg_rent));

        record.setValue(i++, QVariant(eka_place_eka));
        record.setValue(i++, QVariant(eka_place));

        bool inserted = eka_model->insertRecord(-1, record);
        if (!inserted)
        {
            dbm->promptSqlErrorMessage(this, eka_model->lastError());
        }
        else
        {
            QString title = "Gerai";
            QString message = "Duomenys išsaugoti";
            QMessageBox::information(this, title, message);
            this->close();
        }
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

void NaujasEka::on_checkBox_gar_stateChanged(int checked)
{
    if (checked)
        {
            ui->dateEdit_gar->setEnabled(true);
            ui->dateEdit_nuom->setEnabled(false);
            ui->checkBox_nuom->setEnabled(false);
        }
    else
        {
            ui->dateEdit_gar->setEnabled(false);
            QDate date = QDate::currentDate();
            date = date.addYears(1);
            ui->dateEdit_gar->setDate(date);
            ui->checkBox_nuom->setEnabled(true);
        }
}

void NaujasEka::on_checkBox_nuom_stateChanged(int checked)
{
    if (checked)
        {
            ui->dateEdit_nuom->setEnabled(true);
            ui->dateEdit_gar->setEnabled(false);
            ui->checkBox_gar->setEnabled(false);
        }
    else
        {
            ui->dateEdit_nuom->setEnabled(false);
            QDate date = QDate::currentDate();
            ui->dateEdit_nuom->setDate(date);
            ui->checkBox_gar->setEnabled(true);
        }
}

void NaujasEka::on_checkBox_place_stateChanged(int checked)
{
    if (checked)
        {
            ui->checkBox_place->setText("Kaimas");
        }
    else
        {
            ui->checkBox_place->setText("Miestas");
        }
}
