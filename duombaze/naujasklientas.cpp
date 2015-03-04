#include "naujasklientas.h"
#include "ui_naujasklientas.h"
#include "settings.h"
#include "databasemanager.h"

#include <QString>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>

NaujasKlientas::NaujasKlientas(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::NaujasKlientas)
{
    this->dbm = dbm;
    ui->setupUi(this);
    QWidget::setWindowTitle(Settings::TEXT_NAUJASKLIENTAS_NAME);

    table_model = new QSqlTableModel(ui->tableView, dbm->getDatabase());

    table_model->setTable(Settings::CLIENT_TABLE);
    table_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    table_model->select();

    table_model->setHeaderData(0, Qt::Horizontal, "Kliento ID");
    table_model->setHeaderData(1, Qt::Horizontal, "Kliento pavadinimas");
    table_model->setHeaderData(2, Qt::Horizontal, "Kliento kodas");
    table_model->setHeaderData(3, Qt::Horizontal, "Kliento PVM kodas");
    table_model->setHeaderData(4, Qt::Horizontal, "Kliento adresas");
    table_model->setHeaderData(5, Qt::Horizontal, "Kliento telefonas");
    table_model->setHeaderData(6, Qt::Horizontal, "Kliento informacija");

    ui->tableView->setModel(table_model);
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

    QSqlRecord record = table_model->record();
    //record.setValue(0, //cia reikia primary key suzinot arba kazkaip sutaisyt);
    record.setValue(1, QVariant(client_name));
    record.setValue(2, QVariant(client_code));
    record.setValue(3, QVariant(client_vat));
    record.setValue(4, QVariant(client_address));
    record.setValue(5, QVariant(client_telephone));
    record.setValue(6, QVariant(client_addition_info));

    table_model->insertRecord(-1, record);
}
