#include "naujasmodelis.h"
#include "ui_naujasmodelis.h"

#include "settings.h"
#include "databasemanager.h"

#include <QString>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>

NaujasModelis::NaujasModelis(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::NaujasModelis)
{
    this->dbm = dbm;
    ui->setupUi(this);
    QWidget::setWindowTitle(Settings::TEXT_NAUJASMODELIS_NAME);

    table_model = new QSqlTableModel(ui->tableView, dbm->getDatabase());

    table_model->setTable(Settings::MODEL_TABLE);
    table_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    table_model->select();

    table_model->setHeaderData(0, Qt::Horizontal, "Modelio ID");
    table_model->setHeaderData(1, Qt::Horizontal, "Kliento pavadinimas");
    table_model->setHeaderData(2, Qt::Horizontal, "Modelio raidės");

    ui->tableView->setModel(table_model);
}

NaujasModelis::~NaujasModelis()
{
    delete ui;
}

void NaujasModelis::on_pushButton_clicked()
{
    QString model_name = ui->lineEdit->text();
    QString model_letters = ui->lineEdit_2->text();

    QSqlRecord record = table_model->record();
    record.remove(0);
    record.setValue(0, QVariant(model_name));
    record.setValue(1, QVariant(model_letters));

    table_model->insertRecord(-1, record);

    table_model->dumpObjectTree();
}
