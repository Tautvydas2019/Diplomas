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
    //TODO: conversion!!!
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
