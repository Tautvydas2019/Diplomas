#include "naujasklientas.h"
#include "ui_naujasklientas.h"
#include "settings.h"
#include "databasemanager.h"

#include <QMap>
#include <QDebug>
#include <QString>

NaujasKlientas::NaujasKlientas(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::NaujasKlientas)
{
    this->dbm = dbm;
    ui->setupUi(this);
    QWidget::setWindowTitle(Settings::TEXT_NAUJASKLIENTAS_NAME);

    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Kliento pavadinimas"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Kliento kodas"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Kliento PVM kodas"));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Kliento adresas"));
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Kliento telefonas"));
    ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Kliento informacija"));


    auto results = dbm->getClient();
    ui->tableWidget->setRowCount(results.size());
    int row_number = 0;
    for (auto &row : results)
    {
        qDebug() << row;
        QTableWidgetItem *column1 = new QTableWidgetItem(row["name"]);
        column1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row_number, 0, column1);

        QTableWidgetItem *column2 = new QTableWidgetItem(row["code"]);
        column2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row_number, 1 , column2);

        QTableWidgetItem *column3 = new QTableWidgetItem(row["vat"]);
        column3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row_number, 2 , column3);

        QTableWidgetItem *column4 = new QTableWidgetItem(row["address"]);
        column4->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row_number, 3 , column4);

        QTableWidgetItem *column5 = new QTableWidgetItem(row["telephone"]);
        column5->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row_number, 4 , column5);

        QTableWidgetItem *column6 = new QTableWidgetItem(row["additional_info"]);
        column6->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row_number, 5 , column6);
        row_number++;
    }


}



NaujasKlientas::~NaujasKlientas()
{
    delete ui;
}

void NaujasKlientas::on_pushButton_clicked()
{
    QString client_name = ui->lpavadinimas->text();
    QString client_code = ui->lkodas->text();
    QString client_vat = ui->lpvm->text();
    QString client_address = ui->ladresas->text();
    QString client_telephone = ui->ltelefonas->text();
    QString client_addition_info = ui->linformaciha->text();
    dbm->insertClient(client_name, client_code, client_vat, client_address, client_telephone, client_addition_info);
}
