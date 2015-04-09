#include "klientupaieska.h"
#include "ui_klientupaieska.h"
#include "settings.h"
#include "databasemanager.h"
#include "naujasklientas.h"
#include "naujaseka.h"
#include "myqsqlrelationaltablemodel.h"
#include "ekaediting.h"

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
    table_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    table_model->select();

    table_model_eka = new MyQSqlRelationalTableModel(ui->tableView_2, dbm->getDatabase());
    table_model_eka->setTable(Settings::EKA_TABLE);
    table_model_eka->setRelation(1, QSqlRelation(Settings::MODEL_TABLE, "model_id", "name"));
    table_model_eka->setRelation(2, QSqlRelation(Settings::CLIENT_TABLE, "client_id", "name"));
    table_model_eka->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    table_model_eka->setEditStrategy(QSqlTableModel::OnManualSubmit);
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
    table_model_eka->setHeaderData(8, Qt::Horizontal, "Garantija");
    table_model_eka->setHeaderData(9, Qt::Horizontal, "Nuoma");
    table_model_eka->setHeaderData(10, Qt::Horizontal, "EKA vieta");
    table_model_eka->setHeaderData(11, Qt::Horizontal, "Būsena");

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

    for (int i = 0; i < 13; i++){
        ui->tableView_2->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }


    ui->tableView_2->hideColumn(0);
    ui->tableView_2->hideColumn(12);
    ui->tableView_2->hideColumn(13);

    ui->line_pavadinimas->setReadOnly(true);
    ui->lineEdit_kodas->setReadOnly(true);
    ui->lineEdit_pvm->setReadOnly(true);
    ui->lineEdit_adresas->setReadOnly(true);
    ui->lineEdit_miestas->setReadOnly(true);
    ui->textEdit_informacija->setReadOnly(true);
    ui->lineEdit_telefonas->setReadOnly(true);

    ui->pushButton->setEnabled(false);
    ui->pushButton_4->setText("Redaguoti");

    current_record = QSqlRecord();
    current_record_eka = QSqlRecord();

    isEditButton = true;
    ui->pushButton_atsaukti->setHidden(true);
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
                table_model->submitAll();
                table_model->select();
        }
    }
}

void KlientuPaieska::on_tableView_clicked(const QModelIndex &index)
{
    updateTextEdits(index);
}


void KlientuPaieska::on_pushButton_4_clicked()
{
    QModelIndexList selected_indexes = ui->tableView->selectionModel()->selection().indexes();
    if (isEditButton)
    {
        if (selected_indexes.length() > 0) {
            ui->line_pavadinimas->setReadOnly(false);
            ui->lineEdit_kodas->setReadOnly(false);
            ui->lineEdit_pvm->setReadOnly(false);
            ui->lineEdit_adresas->setReadOnly(false);
            ui->lineEdit_miestas->setReadOnly(false);
            ui->textEdit_informacija->setReadOnly(false);
            ui->lineEdit_telefonas->setReadOnly(false);
            ui->tableView->setDisabled(true);

            isEditButton = false;
            ui->pushButton_4->setText("Išsaugoti");
            ui->pushButton_atsaukti->setHidden(false);
        } else {
            //notice kad nieko nepazymeta
        }
    }
    else
    {
        ui->line_pavadinimas->setReadOnly(true);
        ui->lineEdit_kodas->setReadOnly(true);
        ui->lineEdit_pvm->setReadOnly(true);
        ui->lineEdit_adresas->setReadOnly(true);
        ui->lineEdit_miestas->setReadOnly(true);
        ui->textEdit_informacija->setReadOnly(true);
        ui->lineEdit_telefonas->setReadOnly(true);
        ui->tableView->setDisabled(false);

        if (selected_indexes.length() > 0) {
            QModelIndex selected_index = selected_indexes.at(0);
            int selected_row = selected_index.row();

            int i = 1;
            QVariant client_name = QVariant(ui->line_pavadinimas->text());
            QModelIndex cn_index = table_model->index(selected_row, i++);
            table_model->setData(cn_index, client_name);

            QVariant code = QVariant(ui->lineEdit_kodas->text());
            QModelIndex c_index = table_model->index(selected_row, i++);
            table_model->setData(c_index, code);

            QVariant vat = QVariant(ui->lineEdit_pvm->text());
            QModelIndex vat_index = table_model->index(selected_row, i++);
            table_model->setData(vat_index, vat);

            QVariant address = QVariant(ui->lineEdit_adresas->text());
            QModelIndex a_index = table_model->index(selected_row, i++);
            table_model->setData(a_index, address);

            QVariant tel_number = QVariant(ui->lineEdit_telefonas->text());
            QModelIndex tn_index = table_model->index(selected_row, i++);
            table_model->setData(tn_index, tel_number);

            QVariant add_info = QVariant(ui->textEdit_informacija->toPlainText());
            QModelIndex ai_index = table_model->index(selected_row, i++);
            table_model->setData(ai_index, add_info);

            QVariant city = QVariant(ui->lineEdit_miestas->text());
            QModelIndex ct_index = table_model->index(selected_row, i++);
            table_model->setData(ct_index, city);

            table_model->submitAll();
            table_model->select();
        }
        ui->pushButton_atsaukti->setHidden(true);
        ui->pushButton_4->setText("Redaguoti");
        isEditButton = true;
    }
}

void KlientuPaieska::on_pushButton_atsaukti_clicked()
{

    ui->line_pavadinimas->setReadOnly(true);
    ui->lineEdit_kodas->setReadOnly(true);
    ui->lineEdit_pvm->setReadOnly(true);
    ui->lineEdit_adresas->setReadOnly(true);
    ui->lineEdit_miestas->setReadOnly(true);
    ui->textEdit_informacija->setReadOnly(true);
    ui->lineEdit_telefonas->setReadOnly(true);
    ui->tableView->setDisabled(false);

    QModelIndexList selected_indexes = ui->tableView->selectionModel()->selection().indexes();
    if (selected_indexes.length() > 0) {
        QModelIndex selected_index = selected_indexes.at(0);
        updateTextEdits(selected_index);
     }

    ui->pushButton_atsaukti->setHidden(true);
    ui->pushButton_4->setText("Redaguoti");
    isEditButton = true;
}

void KlientuPaieska::updateTextEdits(const QModelIndex &index) {
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
}
