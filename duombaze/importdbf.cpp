#include "importdbf.h"
#include "ui_importdbf.h"
#include "qdbfrecord.h"
#include "qdbftablemodel.h"
#include "qdbftable.h"
#include "settings.h"
#include "qdbfrecord.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVariant>

using namespace QDbf;

ImportDbf::ImportDbf(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::ImportDbf)
{
    this->dbm = dbm;
    ui->setupUi(this);

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
}

ImportDbf::~ImportDbf()
{
    delete ui;
}

void ImportDbf::on_toolButton_eka_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Pasirinkti eka .dbf failą");
    if (eka_table.open(file_name, QDbfTable::ReadOnly) && eka_table.next() && eka_table.record().count() == 21)
    {
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
    dbm->createTempTables();
    eka_table.open(ui->lineEdit_eka->text());
    int dbf_size = eka_table.size();
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(dbf_size);
    int current_row = 0;
    dbm->getDatabase().transaction();
    while (eka_table.next())
    {
        dbm->importDbfEkaRecord(eka_table.record());
        current_row++;
        ui->progressBar->setValue(current_row);
    }
    dbm->getDatabase().commit();
    eka_model->setTable(Settings::TEMP_EKA_TABLE);
    ui->tableView_eka->setModel(eka_model);
    eka_model->select();
    ui->progressBar->setVisible(false);
}

void ImportDbf::on_pushButton_client_clicked()
{
    dbm->createTempTables();
    client_table.open(ui->lineEdit_client->text());
    int dbf_size = client_table.size();
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(dbf_size);
    int current_row = 0;
    dbm->getDatabase().transaction();
    while (client_table.next())
    {
        dbm->importDbfClientRecord(client_table.record());
        current_row++;
        ui->progressBar->setValue(current_row);
    }
    dbm->getDatabase().commit();
    client_model->setTable(Settings::TEMP_CLIENT_TABLE);
    ui->tableView_client->setModel(client_model);
    client_model->select();
    ui->progressBar->setVisible(false);
}

void ImportDbf::on_pushButton_model_clicked()
{
    dbm->createTempTables();
    model_table.open(ui->lineEdit_model->text());
    int dbf_size = model_table.size();
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(dbf_size);
    int current_row = 0;
    dbm->getDatabase().transaction();
    while (model_table.next())
    {
        dbm->importDbfModelRecord(model_table.record());
        current_row++;
        ui->progressBar->setValue(current_row);
    }
    dbm->getDatabase().commit();
    model_model->setTable(Settings::TEMP_MODEL_TABLE);
    ui->tableView_model->setModel(model_model);
    model_model->select();
    ui->progressBar->setVisible(false);
}

void ImportDbf::on_pushButton_addToDb_clicked()
{
    //TODO: conversion!!!
}


