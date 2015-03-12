#include "klientupaieska.h"
#include "ui_klientupaieska.h"
#include "settings.h"
#include "databasemanager.h"

#include <QString>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

KlientuPaieska::KlientuPaieska(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::KlientuPaieska)
{
    this->dbm = dbm;
    ui->setupUi(this);
    QWidget::setWindowTitle(Settings::TEXT_KLIENTUPAIESKA_NAME);

    table_model = new QSqlTableModel(ui->tableView, dbm->getDatabase());

    table_model->setTable(Settings::CLIENT_TABLE);
    table_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    table_model->select();

    table_model->setHeaderData(1, Qt::Horizontal, "Kliento pavadinimas");
    table_model->setHeaderData(2, Qt::Horizontal, "Įm. kodas");
    table_model->setHeaderData(7, Qt::Horizontal, "Miestas");

    ui->tableView->setModel(table_model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Interactive);

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(3);
    ui->tableView->hideColumn(4);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);

    ui->line_pavadinimas->setReadOnly("");
    ui->lineEdit_kodas->setReadOnly("");
    ui->lineEdit_pvm->setReadOnly("");
    ui->lineEdit_adresas->setReadOnly("");
    ui->lineEdit_miestas->setReadOnly("");
    ui->lineEdit_informacija->setReadOnly("");
    ui->lineEdit_telefonas->setReadOnly("");

}

KlientuPaieska::~KlientuPaieska()
{
    delete ui;
}

void KlientuPaieska::on_tableView_activated(const QModelIndex &index)
{
    QString sql = ui->tableView->model()->data(index).toString();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM `client` WHERE name = '"+sql+"' or code = '"+sql+"' or city = '"+sql+"'");


    if(qry.exec())
    {
        while(qry.next())
        {
        ui->line_pavadinimas->setText(qry.value(1).toString());
        ui->lineEdit_kodas->setText(qry.value(2).toString());
        ui->lineEdit_pvm->setText(qry.value(3).toString());
        ui->lineEdit_adresas->setText(qry.value(4).toString());
        ui->lineEdit_miestas->setText(qry.value(5).toString());
        ui->lineEdit_telefonas->setText(qry.value(6).toString());
        ui->lineEdit_informacija->setText(qry.value(7).toString());
        }
    }
    else
    {
        QMessageBox::critical(this,tr("error::"),qry.lastError().text());
    }
}
