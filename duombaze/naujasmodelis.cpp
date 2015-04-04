#include "naujasmodelis.h"
#include "ui_naujasmodelis.h"

#include "settings.h"
#include "databasemanager.h"

#include <QWidget>
#include <QDialog>
#include <QSqlTableModel>
#include <Qt>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QString>
#include <QMessageBox>
#include <QSqlRecord>
#include <QVariant>
#include <QModelIndexList>
#include <QList>
#include <QModelIndex>

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
    table_model->setSort(2, Qt::AscendingOrder);
    table_model->select();

    table_model->setHeaderData(1, Qt::Horizontal, "Modelio pavadinimas");
    table_model->setHeaderData(2, Qt::Horizontal, "Modelio ID");

    ui->tableView->setModel(table_model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
     ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->tableView->hideColumn(0);

    ui->pushButton_2->setEnabled(false);
}

NaujasModelis::~NaujasModelis()
{
    delete ui;
}

void NaujasModelis::on_pushButton_clicked()
{
    QString model_name = ui->lineEdit->text();
    QString model_letters = ui->lineEdit_2->text();


    if (model_name.isEmpty() || model_letters.isEmpty())
    {
        QString title = "Neįvesti duomenys";
        QString message = "Turite įvesti modelio pavadinimą ir modelio ID!";
        QMessageBox::warning(this, title, message);
    }
    else
    {
        QSqlRecord record = table_model->record();
        record.remove(0);
        record.setValue(0, QVariant(model_name));
        record.setValue(1, QVariant(model_letters));

        bool inserted = table_model->insertRecord(-1, record);
        if (!inserted)
        {
            dbm->promptSqlErrorMessage(this, table_model->lastError());
        }
        else
        {
            ui->lineEdit->setText("");
            ui->lineEdit_2->setText("");
        }
        table_model->sort(2, Qt::AscendingOrder);
        QString title = "Gerai";
        QString message = "Duomenys išsaugoti";
        QMessageBox::information(this, title, message);
    }
}


void NaujasModelis::on_pushButton_2_clicked()
{
    QModelIndexList selected_indexes = ui->tableView->selectionModel()->selection().indexes();
        if (selected_indexes.length() > 0) {

            QModelIndex selected_index = selected_indexes.at(0);
            int selected_row = selected_index.row();
            QModelIndex target_index = table_model->index(selected_row, 1);
            QString selected_model_name = table_model->data(target_index).toString();

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,
                                          "Naikinti įrašą \"" + selected_model_name + "\"",
                                          "Tikrai naikinti įrašą \"" + selected_model_name + "\"?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                table_model->removeRow(selected_row);
                table_model->select();
        }
    }
}

void NaujasModelis::on_checkBox_stateChanged(int checked)
{
    if (checked)
    {
        ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked |
            QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
        ui->tableView->clearSelection();
        ui->pushButton_2->setEnabled(true);
    }
    else
    {
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView->clearSelection();
        ui->pushButton_2->setEnabled(false);
    }
}
