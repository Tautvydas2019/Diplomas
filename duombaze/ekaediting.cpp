#include "ekaediting.h"
#include "ui_ekaediting.h"
#include "klientupaieska.h"
#include "ui_klientupaieska.h"
#include "settings.h"
#include "databasemanager.h"
#include "myqsqlrelationaltablemodel.h"

#include <QString>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDate>

EkaEditing::EkaEditing(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::EkaEditing)
{
    this->dbm = dbm;
    ui->setupUi(this);
    QWidget::setWindowTitle(Settings::TEXT_EKAEDITING_NAME);

    table_model = new MyQSqlRelationalTableModel(ui->tableView, dbm->getDatabase());
    table_model->setTable(Settings::EKA_TABLE);
    table_model->setRelation(1, QSqlRelation(Settings::MODEL_TABLE, "model_id", "name"));
    table_model->setRelation(2, QSqlRelation(Settings::CLIENT_TABLE, "client_id", "name"));
    table_model->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    table_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    table_model->select();

    client_info_model = new QSqlQueryModel(this);

    ui->tableView->setModel(table_model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    table_model->setHeaderData(3, Qt::Horizontal, "EKA numeris");
    table_model->setHeaderData(10, Qt::Horizontal, "Stovėjimo adresas");
    table_model->setHeaderData(11, Qt::Horizontal, "Veikla");

    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(10, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(11, QHeaderView::ResizeToContents);

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(4);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);
    ui->tableView->hideColumn(7);
    ui->tableView->hideColumn(8);
    ui->tableView->hideColumn(9);
    ui->tableView->hideColumn(12);
    ui->tableView->hideColumn(13);

    ui->lineEdit_address->setReadOnly(true);
    ui->lineEdit_certificate->setReadOnly(true);
    ui->lineEdit_city->setReadOnly(true);
    ui->lineEdit_client->setReadOnly(true);
    ui->lineEdit_code->setReadOnly(true);
    ui->lineEdit_ekaplace->setReadOnly(true);
    ui->lineEdit_howmany->setReadOnly(true);
    ui->lineEdit_model->setReadOnly(true);
    ui->lineEdit_serial->setReadOnly(true);
    ui->lineEdit_tel->setReadOnly(true);
    ui->lineEdit_vat->setReadOnly(true);

    ui->dateEdit_prof->setDisabled(true);
    ui->dateEdit_reg->setDisabled(true);
    ui->dateEdit_rent->setDisabled(true);
    ui->dateEdit_warranty->setDisabled(true);
    ui->checkBox_rent->setDisabled(true);
    ui->checkBox_warranty->setDisabled(true);
    ui->checkBox_status->setDisabled(true);
    ui->toolButton_client2->setDisabled(true);

    ui->lineEdit_status->setReadOnly(true);


    ui->pushButton_edit->setText("Redaguoti");
    isEditButton = true;
    ui->pushButton_atsaukti->setHidden(true);

    current_record = QSqlRecord();
    client_record = QSqlRecord();
}


EkaEditing::~EkaEditing()
{
    delete ui;
}

void EkaEditing::on_lineEdit_search_textChanged(const QString &search_keyword)
{
    QString filter = "`serial_number` LIKE '%" + search_keyword + "%' OR `place_eka` LIKE '%" + search_keyword +
            "%' OR `status` LIKE '%" + search_keyword + "%'";
    table_model->setFilter(filter);
    table_model->select();
}



void EkaEditing::on_tableView_activated(const QModelIndex &index)
{
    int row = index.row();
    QSqlRecord record = table_model->record(row);

    current_record = record;

    ui->lineEdit_client->setText(record.value(2).toString());
    ui->lineEdit_model->setText(record.value(1).toString());
    ui->lineEdit_serial->setText(record.value(3).toString());
    ui->lineEdit_certificate->setText(record.value(4).toString());
    ui->lineEdit_howmany->setText(record.value(5).toString());
    ui->dateEdit_reg->setDate(record.value(6).toDate());
    ui->dateEdit_prof->setDate(record.value(7).toDate());
    QDate warranty_date = (record.value(8).toString().isEmpty()) ? QDate(2000, 1, 1) : record.value(8).toDate();
    ui->dateEdit_warranty->setDate(warranty_date);
    QDate rent_date = (record.value(9).toString().isEmpty()) ? QDate(2000, 1, 1) : record.value(9).toDate();
    ui->dateEdit_rent->setDate(rent_date);
    ui->lineEdit_ekaplace->setText(record.value(10).toString());
    ui->lineEdit_status->setText(record.value(11).toString());

    QString eka_id = record.value(0).toString();
    client_info_model->setQuery("SELECT " + Settings::CLIENT_TABLE + ".* FROM `" + Settings::EKA_TABLE + "` LEFT JOIN " + Settings::CLIENT_TABLE
        + " ON " + Settings::EKA_TABLE + ".client_id = " + Settings::CLIENT_TABLE + ".client_id WHERE `eka_id` = '" + eka_id + "';");

    QSqlRecord client = client_info_model->record(0);
    client_record = client;
    ui->lineEdit_client->setText(client.value(1).toString());
    ui->lineEdit_code->setText(client.value(2).toString());
    ui->lineEdit_vat->setText(client.value(3).toString());
    ui->lineEdit_address->setText(client.value(4).toString());
    ui->lineEdit_city->setText(client.value(7).toString());
    ui->lineEdit_tel->setText(client.value(5).toString());
}

void EkaEditing::on_pushButton_delete_clicked()
{
    QModelIndexList selected_indexes = ui->tableView->selectionModel()->selection().indexes();
        if (selected_indexes.length() > 0) {

            QModelIndex selected_index = selected_indexes.at(0);
            int selected_row = selected_index.row();
            const QModelIndex target_index_serial = table_model->index(selected_row, 3);
            const QModelIndex target_index_model = table_model->index(selected_row, 1);
            QString selected_serial_name = table_model->data(target_index_serial).toString();
            QString selected_model_name = table_model->data(target_index_model).toString();

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,
                                          "Naikinti įrašą \"" + selected_model_name + "\"",
                                          "Tikrai naikinti EKA \"" + selected_model_name + ", korpuso numeris: " + selected_serial_name +"\"?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                table_model->removeRow(selected_row);
                table_model->submitAll();
                table_model->select();
             }
        }
}


void EkaEditing::on_pushButton_edit_clicked()
{
    QModelIndexList selected_indexes = ui->tableView->selectionModel()->selection().indexes();
    if (isEditButton)
    {
        if (selected_indexes.length() > 0) {
            ui->lineEdit_address->setReadOnly(true);
            ui->lineEdit_certificate->setReadOnly(false);
            ui->lineEdit_city->setReadOnly(true);
            ui->lineEdit_client->setReadOnly(true);
            ui->lineEdit_code->setReadOnly(true);
            ui->lineEdit_ekaplace->setReadOnly(false);
            ui->lineEdit_howmany->setReadOnly(false);
            ui->lineEdit_model->setReadOnly(true);
            ui->lineEdit_serial->setReadOnly(false);
            ui->lineEdit_tel->setReadOnly(true);
            ui->lineEdit_vat->setReadOnly(true);
            ui->dateEdit_prof->setDisabled(false);
            ui->dateEdit_reg->setDisabled(false);
            ui->dateEdit_rent->setDisabled(false);
            ui->dateEdit_warranty->setDisabled(false);
            ui->lineEdit_status->setReadOnly(true);
            ui->checkBox_rent->setDisabled(false);
            ui->checkBox_warranty->setDisabled(false);
            ui->checkBox_status->setDisabled(false);
            ui->toolButton_client2->setDisabled(false);
            ui->tableView->setDisabled(true);

            isEditButton = false;
            ui->pushButton_edit->setText("Išsaugoti");
            ui->pushButton_atsaukti->setHidden(false);
        } else {
            //notice kad nieko nepazymeta
        }
    }
    else
    {
        ui->lineEdit_address->setReadOnly(true);
        ui->lineEdit_certificate->setReadOnly(true);
        ui->lineEdit_city->setReadOnly(true);
        ui->lineEdit_client->setReadOnly(true);
        ui->lineEdit_code->setReadOnly(true);
        ui->lineEdit_ekaplace->setReadOnly(true);
        ui->lineEdit_howmany->setReadOnly(true);
        ui->lineEdit_model->setReadOnly(true);
        ui->lineEdit_serial->setReadOnly(true);
        ui->lineEdit_tel->setReadOnly(true);
        ui->lineEdit_vat->setReadOnly(true);
        ui->dateEdit_prof->setDisabled(true);
        ui->dateEdit_reg->setDisabled(true);
        ui->dateEdit_rent->setDisabled(true);
        ui->dateEdit_warranty->setDisabled(true);
        ui->lineEdit_status->setReadOnly(true);
        ui->checkBox_rent->setDisabled(true);
        ui->checkBox_warranty->setDisabled(true);
        ui->checkBox_status->setDisabled(true);
        ui->toolButton_client2->setDisabled(true);
        ui->tableView->setDisabled(false);

        if (selected_indexes.length() > 0) {
            QModelIndex selected_index = selected_indexes.at(0);
            int selected_row = selected_index.row();

            QVariant client = client_record.value(0);
            QModelIndex client_index = table_model->index(selected_row, 2);
            table_model->setData(client_index, client);

            QVariant serial = QVariant(ui->lineEdit_serial->text());
            QModelIndex serial_index = table_model->index(selected_row, 3);
            table_model->setData(serial_index, serial);

            QVariant certificate = QVariant(ui->lineEdit_certificate->text());
            QModelIndex certificate_index = table_model->index(selected_row, 4);
            table_model->setData(certificate_index, certificate);

            QVariant count_of_use = QVariant(ui->lineEdit_howmany->text());
            QModelIndex count_of_use_index = table_model->index(selected_row, 5);
            table_model->setData(count_of_use_index, count_of_use);

            QVariant prof = QVariant(ui->dateEdit_prof->date());
            QModelIndex prof_index = table_model->index(selected_row, 7);
            table_model->setData(prof_index, prof);

            QVariant warranty = QVariant(ui->dateEdit_warranty->date());
            QModelIndex warranty_index = table_model->index(selected_row, 8);
            table_model->setData(warranty_index, warranty);

            QVariant rent = QVariant(ui->dateEdit_rent->date());
            QModelIndex rent_index = table_model->index(selected_row, 9);
            table_model->setData(rent_index, rent);

            QVariant ekaplace = QVariant(ui->lineEdit_ekaplace->text());
            QModelIndex ekaplace_index = table_model->index(selected_row, 10);
            table_model->setData(ekaplace_index, ekaplace);

            QVariant status = QVariant(ui->lineEdit_status->text());
            QModelIndex status_index = table_model->index(selected_row, 11);
            table_model->setData(status_index, status);


            table_model->submitAll();
            table_model->select();
        }
        ui->pushButton_atsaukti->setHidden(true);
        ui->pushButton_edit->setText("Redaguoti");
        isEditButton = true;
    }
}

void EkaEditing::on_pushButton_atsaukti_clicked()
{
    ui->lineEdit_address->setReadOnly(true);
    ui->lineEdit_certificate->setReadOnly(true);
    ui->lineEdit_city->setReadOnly(true);
    ui->lineEdit_client->setReadOnly(true);
    ui->lineEdit_code->setReadOnly(true);
    ui->lineEdit_ekaplace->setReadOnly(true);
    ui->lineEdit_howmany->setReadOnly(true);
    ui->lineEdit_model->setReadOnly(true);
    ui->lineEdit_serial->setReadOnly(true);
    ui->lineEdit_tel->setReadOnly(true);
    ui->lineEdit_vat->setReadOnly(true);
    ui->dateEdit_prof->setDisabled(true);
    ui->dateEdit_reg->setDisabled(true);
    ui->dateEdit_rent->setDisabled(true);
    ui->dateEdit_warranty->setDisabled(true);
    ui->lineEdit_status->setReadOnly(true);
    ui->checkBox_rent->setDisabled(true);
    ui->checkBox_warranty->setDisabled(true);
    ui->checkBox_status->setDisabled(true);
    ui->toolButton_client2->setDisabled(true);
    ui->tableView->setDisabled(false);

    QModelIndexList selected_indexes = ui->tableView->selectionModel()->selection().indexes();
    if (selected_indexes.length() > 0) {
        QModelIndex selected_index = selected_indexes.at(0);
        updateTextEdits(selected_index);
     }

    ui->pushButton_atsaukti->setHidden(true);
    ui->pushButton_edit->setText("Redaguoti");
    isEditButton = true;
}

void EkaEditing::updateTextEdits(const QModelIndex &index) {
    int row = index.row();
    QSqlRecord record = table_model->record(row);

    current_record = record;

    ui->lineEdit_model->setText(record.value(1).toString());
    ui->lineEdit_serial->setText(record.value(3).toString());
    ui->lineEdit_certificate->setText(record.value(4).toString());
    ui->lineEdit_howmany->setText(record.value(5).toString());
    ui->dateEdit_reg->setDate(record.value(6).toDate());
    ui->dateEdit_prof->setDate(record.value(7).toDate());
    ui->dateEdit_warranty->setDate(record.value(8).toDate());
    ui->dateEdit_rent->setDate(record.value(9).toDate());
    ui->lineEdit_ekaplace->setText(record.value(10).toString());
    ui->lineEdit_status->setText(record.value(11).toString());

}

void EkaEditing::setClient()
{
    QSqlRecord client = client_search_dialog->getCurrentClient();
    if (!client.isEmpty())
    {
        client_record = client;
        ui->lineEdit_client->setText(client.value(1).toString());
        ui->lineEdit_code->setText(client.value(2).toString());
        ui->lineEdit_vat->setText(client.value(3).toString());
        ui->lineEdit_address->setText(client.value(4).toString());
        ui->lineEdit_city->setText(client.value(7).toString());
        ui->lineEdit_tel->setText(client.value(5).toString());
    }
}


void EkaEditing::on_toolButton_client2_clicked()
{
    client_search_dialog = new KlientuPaieska(this, dbm);
    client_search_dialog->setModal(true);
    connect(client_search_dialog, SIGNAL(accepted()), this, SLOT(setClient()));
    client_search_dialog->exec();
}

void EkaEditing::on_checkBox_warranty_stateChanged(int checked)
{

    if (checked)
        {
            ui->dateEdit_warranty->setEnabled(true);
            QDate date = QDate::currentDate();
            date = date.addYears(1);
            ui->dateEdit_warranty->setDate(date);
            ui->dateEdit_rent->setEnabled(false);
            ui->checkBox_rent->setEnabled(false);
        }
    else
        {
            QDate date;
            date.setDate(2001, 1, 1);
            ui->dateEdit_warranty->setDate(date);
            ui->dateEdit_warranty->setEnabled(false);
            ui->checkBox_rent->setEnabled(true);
        }

}

void EkaEditing::on_checkBox_rent_stateChanged(int checked)
{
    if (checked)
        {
            ui->dateEdit_rent->setEnabled(true);
            QDate date = QDate::currentDate();
            date = date.addYears(1);
            ui->dateEdit_rent->setDate(date);
            ui->dateEdit_warranty->setEnabled(false);
            ui->checkBox_warranty->setEnabled(false);
        }
    else
        {
           QDate date;
           date.setDate(2001, 1, 1);
           ui->dateEdit_rent->setDate(date);
           ui->dateEdit_warranty->setEnabled(false);
           ui->checkBox_rent->setEnabled(true);

        }
}

void EkaEditing::on_checkBox_status_stateChanged(int checked)
{
    if (checked){
        ui->lineEdit_status->setText("Nedirba");
    }
    else {
        ui->lineEdit_status->setText("Dirba");
    }
}
