#include "naujasklientas.h"
#include "ui_naujasklientas.h"

#include "settings.h"
#include "databasemanager.h"

#include <QString>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

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
    table_model->setHeaderData(7, Qt::Horizontal, "Kliento miestas");

    ui->tableView->setModel(table_model);
    ui->tableView->hide();
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
    QString client_city = ui->lmiestas->text();

    if (client_name.isEmpty())
    {
        QString title = "Neįvesti duomenys";
        QString message = "Turite įvesti Kliento pavadinimą!";
        QMessageBox::warning(this, title, message);
    }
    else
    {
        QSqlRecord record = table_model->record();
        record.remove(0);
        record.setValue(0, QVariant(client_name));
        record.setValue(1, QVariant(client_code));
        record.setValue(2, QVariant(client_vat));
        record.setValue(3, QVariant(client_address));
        record.setValue(4, QVariant(client_telephone));
        record.setValue(5, QVariant(client_addition_info));
        record.setValue(6, QVariant(client_city));

        //table_model->insertRecord(-1, record);
        bool inserted = table_model->insertRecord(-1, record);
        if (!inserted)
        {
            QSqlError error = table_model->lastError();
            if (error.number() == 19)
            {
                QString title = "Pasikartojantys duomenys";
                QString message = "Įvedėte duomenis kurie jau yra duomenų bazėje";
                QMessageBox::critical(this, title, message);
            }
            else
            {
                QString title = "Nežinoma klaida";
                QString message = error.text();
                QMessageBox::critical(this, title, message);
            }
        }
        else
        {
            ui->lpavadinimas->setText("");
            ui->lkodas->setText("");
            ui->lpvm->setText("");
            ui->ladresas->setText("");
            ui->ltelefonas->setText("");
            ui->linformaciha->setText("");
            ui->lmiestas->setText("");
        }
    }
}
