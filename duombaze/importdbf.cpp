#include "importdbf.h"
#include "ui_importdbf.h"
#include "databasemanager.h"
#include "qdbftablemodel.h"

#include <QFileDialog>

ImportDbf::ImportDbf(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::ImportDbf)
{
    this->dbm = dbm;
    ui->setupUi(this);

    ui->lineEdit_client->setDisabled(true);
    ui->lineEdit_eka->setDisabled(true);
    ui->radioButton->setChecked(true);

    dbf_client_model = new QDbf::QDbfTableModel(this);
    dbf_eka_model = new QDbf::QDbfTableModel(this);
}

ImportDbf::~ImportDbf()
{
    delete ui;
}

void ImportDbf::on_toolButton_eka_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Pasirinkti eka .dbf failą");
    ui->lineEdit_eka->setText(fileName);
    if (dbf_eka_model->open(fileName)) {
        ui->tableView_dbf_eka->setModel(dbf_eka_model);
    }
}

void ImportDbf::on_toolButton_client_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Pasirinkti kliento .dbf failą");
    ui->lineEdit_client->setText(fileName);
    if (dbf_client_model->open(fileName)) {
        ui->tableView_dbf_client->setModel(dbf_client_model);
    }
}

void ImportDbf::on_pushButton_clicked()
{
    for (int row = 0; i < dbf_client_model->rowCount(); i++) {

    }
}
