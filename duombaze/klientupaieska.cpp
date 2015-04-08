#include "klientupaieska.h"
#include "ui_klientupaieska.h"
#include "settings.h"
#include "databasemanager.h"
#include "naujasklientas.h"
#include "naujaseka.h"
#include "ui_naujaseka.h"

#include <QString>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRelation>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>

#include <QDebug>


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

    table_model_eka = new QSqlRelationalTableModel(ui->tableView_2, dbm->getDatabase());
    table_model_eka->setTable(Settings::EKA_TABLE);
    table_model_eka->setRelation(1, QSqlRelation(Settings::MODEL_TABLE, "model_id", "name"));
    table_model_eka->setRelation(2, QSqlRelation(Settings::CLIENT_TABLE, "client_id", "name"));
    table_model_eka->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    table_model_eka->setEditStrategy(QSqlTableModel::OnFieldChange);
    table_model_eka->setFilter("0=1");
    table_model_eka->select();

    table_model->setHeaderData(1, Qt::Horizontal, "Kliento pavadinimas");
    table_model->setHeaderData(2, Qt::Horizontal, "Įm. kodas");
    table_model->setHeaderData(7, Qt::Horizontal, "Miestas");

    table_model_eka->setHeaderData(1, Qt::Horizontal, "Modelis");
    table_model_eka->setHeaderData(2, Qt::Horizontal, "Klientas ");
    table_model_eka->setHeaderData(3, Qt::Horizontal, "EKA numeris");
    table_model_eka->setHeaderData(4, Qt::Horizontal, "Pasas");
    table_model_eka->setHeaderData(5, Qt::Horizontal, "Registravimų kiekis");
    table_model_eka->setHeaderData(6, Qt::Horizontal, "Registravimo data");
    table_model_eka->setHeaderData(7, Qt::Horizontal, "Profilaktika");
    table_model_eka->setHeaderData(9, Qt::Horizontal, "Garantija");
    table_model_eka->setHeaderData(11, Qt::Horizontal, "Nuoma");
    table_model_eka->setHeaderData(12, Qt::Horizontal, "EKA vieta");
    table_model_eka->setHeaderData(13, Qt::Horizontal, "Būsena");



    ui->tableView->setModel(table_model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(3);
    ui->tableView->hideColumn(4);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);

    ui->tableView_2->setModel(table_model_eka);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView_2->hideColumn(0);
    ui->tableView_2->hideColumn(8);
    ui->tableView_2->hideColumn(10);
    ui->tableView_2->hideColumn(14);
    ui->tableView_2->hideColumn(15);

    ui->line_pavadinimas->setDisabled(true);
    ui->lineEdit_kodas->setDisabled(true);
    ui->lineEdit_pvm->setDisabled(true);
    ui->lineEdit_adresas->setDisabled(true);
    ui->lineEdit_miestas->setDisabled(true);
    ui->textEdit_informacija->setDisabled(true);
    ui->lineEdit_telefonas->setDisabled(true);

    ui->pushButton->setEnabled(false);

    current_record = QSqlRecord();
    current_record_eka = QSqlRecord();

}

KlientuPaieska::~KlientuPaieska()
{
    delete ui;
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
    table_model->select();
}

void KlientuPaieska::on_pushButton_5_clicked()
{
    QModelIndexList selected_indexes = ui->tableView->selectionModel()->selection().indexes();
        if (selected_indexes.length() > 0) {

            QModelIndex selected_index = selected_indexes.at(0);
            int selected_row = selected_index.row();
            QModelIndex target_index = table_model->index(selected_row, 1);
            QString selected_client_name = table_model->data(target_index).toString();

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,
                                          "Naikinti įrašą \"" + selected_client_name + "\"",
                                          "Tikrai naikinti įrašą \"" + selected_client_name + "\"?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                table_model->removeRow(selected_row);
                table_model->select();
        }
    }
}

void KlientuPaieska::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    QSqlRecord record = table_model->record(row);
    QSqlRecord record_eka = table_model_eka->record();

    current_record = record;
    current_record_eka = record_eka;

    ui->line_pavadinimas->setText(record.value(1).toString());
    ui->lineEdit_kodas->setText(record.value(2).toString());
    ui->lineEdit_pvm->setText(record.value(3).toString());
    ui->lineEdit_adresas->setText(record.value(4).toString());
    ui->lineEdit_miestas->setText(record.value(7).toString());
    ui->lineEdit_telefonas->setText(record.value(5).toString());
    ui->textEdit_informacija->setText(record.value(6).toString());
    ui->pushButton->setEnabled(true);

    QString eka_filter = Settings::EKA_TABLE + ".client_id = '" + record.value(0).toString() + "'";
    table_model_eka->setFilter(eka_filter);
    table_model_eka->select();

    QVariant eka_warranty = record.value(8).toString();
    QVariant eka_rent = record.value(10).toString();
     //qDebug() << eka_warranty;
    if (eka_warranty == false)
        {
            ui->tableView_2->hideColumn(9);
            qDebug() << eka_warranty;
        }

    if (eka_rent == false)
        {
            ui->tableView_2->hideColumn(11);
            qDebug() << eka_rent;
        }
}

void KlientuPaieska::on_checkBox_stateChanged(int checked)
{
    if (checked)
    {
        ui->line_pavadinimas->setDisabled(false);
        ui->lineEdit_kodas->setDisabled(false);
        ui->lineEdit_pvm->setDisabled(false);
        ui->lineEdit_adresas->setDisabled(false);
        ui->lineEdit_miestas->setDisabled(false);
        ui->textEdit_informacija->setDisabled(false);
        ui->lineEdit_telefonas->setDisabled(false);
    }
    else
    {
        ui->line_pavadinimas->setDisabled(true);
        ui->lineEdit_kodas->setDisabled(true);
        ui->lineEdit_pvm->setDisabled(true);
        ui->lineEdit_adresas->setDisabled(true);
        ui->lineEdit_miestas->setDisabled(true);
        ui->textEdit_informacija->setDisabled(true);
        ui->lineEdit_telefonas->setDisabled(true);
    }
}
