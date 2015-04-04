#include "ekaediting.h"
#include "ui_ekaediting.h"
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

EkaEditing::EkaEditing(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::EkaEditing)
{
    this->dbm = dbm;
    ui->setupUi(this);
    QWidget::setWindowTitle(Settings::TEXT_EKAEDITING_NAME);

    table_model = new QSqlTableModel(ui->tableView, dbm->getDatabase());
    table_model->setTable(Settings::EKA_TABLE);
    table_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    table_model->select();

    ui->tableView->setModel(table_model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(4);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);
    ui->tableView->hideColumn(7);
    ui->tableView->hideColumn(8);
    ui->tableView->hideColumn(9);
    ui->tableView->hideColumn(10);
    ui->tableView->hideColumn(11);
    ui->tableView->hideColumn(14);
    ui->tableView->hideColumn(15);


    //ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);

}

EkaEditing::~EkaEditing()
{
    delete ui;
}

void EkaEditing::on_lineEdit_textChanged(const QString &search_keyword)
{
    QString filter = "`serial_number` LIKE '%" + search_keyword + "%' OR `status` LIKE '%" + search_keyword
            + "%' OR `place_eka` LIKE '%" + search_keyword + "%'";
    table_model->setFilter(filter);
    table_model->select();
}
