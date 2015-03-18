#include "klientupaieska.h"
#include "ui_klientupaieska.h"
#include "settings.h"
#include "databasemanager.h"
#include "naujasklientas.h"
#include "ui_naujasklientas.h"

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
    table_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
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

    ui->line_pavadinimas->setReadOnly(true);
    ui->lineEdit_kodas->setReadOnly(true);
    ui->lineEdit_pvm->setReadOnly(true);
    ui->lineEdit_adresas->setReadOnly(true);
    ui->lineEdit_miestas->setReadOnly(true);
    ui->textEdit_informacija->setReadOnly(true);
    ui->lineEdit_telefonas->setReadOnly(true);

    ui->pushButton->setEnabled(false);

    current_record = QSqlRecord();

}

KlientuPaieska::~KlientuPaieska()
{
    delete ui;
}

void KlientuPaieska::on_tableView_activated(const QModelIndex &index)
{
    int row = index.row();
    QSqlRecord record = table_model->record(row);

    current_record = record;

    ui->line_pavadinimas->setText(record.value(1).toString());
    ui->lineEdit_kodas->setText(record.value(2).toString());
    ui->lineEdit_pvm->setText(record.value(3).toString());
    ui->lineEdit_adresas->setText(record.value(4).toString());
    ui->lineEdit_miestas->setText(record.value(7).toString());
    ui->lineEdit_telefonas->setText(record.value(5).toString());
    ui->textEdit_informacija->setText(record.value(6).toString());

    ui->pushButton->setEnabled(true);
}

void KlientuPaieska::on_lpaieska_textChanged(const QString &search_keyword)
{
    QString filter = "`name` LIKE '%" + search_keyword + "%' OR `code` LIKE '%" + search_keyword
            + "%' OR `city` LIKE '%" + search_keyword + "%'";
    table_model->setFilter(filter);
    table_model->select();
}

QSqlRecord KlientuPaieska::getCurrentClient()
{
    return current_record;
}

void KlientuPaieska::on_pushButton_3_clicked()
{
    NaujasKlientas nKlientas(this, dbm);
    nKlientas.setModal(true);
    nKlientas.exec();
}
