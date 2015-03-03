#include "naujasmodelis.h"
#include "ui_naujasmodelis.h"
#include "settings.h"
#include "databasemanager.h"

#include <QString>
#include <QMap>
#include <QDebug>

NaujasModelis::NaujasModelis(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::NaujasModelis)
{
    this->dbm = dbm;
    ui->setupUi(this);
    QWidget::setWindowTitle(Settings::TEXT_NAUJASMODELIS_NAME);

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Modelio pavadinimas"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Modelio ID"));

    auto results = dbm->getModels();
    ui->tableWidget->setRowCount(results.size());
    int row_number = 0;
    for (auto &row : results)
    {
        QTableWidgetItem *column1 = new QTableWidgetItem(row["name"]);
        column1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row_number, 0, column1);

        QTableWidgetItem *column2 = new QTableWidgetItem(row["letters"]);
        column2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row_number, 1 , column2);
        row_number++;
    }
}


NaujasModelis::~NaujasModelis()
{
    delete ui;
}

void NaujasModelis::on_pushButton_clicked()
{
    QString model_name = ui->lineEdit->text();
    QString model_letters = ui->lineEdit_2->text();
    dbm->insertModel(model_name, model_letters);
}
