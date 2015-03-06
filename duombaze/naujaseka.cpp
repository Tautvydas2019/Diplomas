#include "naujaseka.h"
#include "ui_naujaseka.h"
#include "naujasmodelis.h"
#include "ui_naujasmodelis.h"

#include "settings.h"
#include "databasemanager.h"

#include <QString>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QComboBox>
#include <QAbstractItemModel>
#include <QSqlQueryModel>

NaujasEka::NaujasEka(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::NaujasEka)
{
    this->dbm = dbm;
    ui->setupUi(this);
    QWidget::setWindowTitle(Settings::TEXT_NAUJASEKA_NAME);

}

NaujasEka::~NaujasEka()
{
    delete ui;
}

void NaujasEka::on_pushButton_save_clicked()
{
    QString eka_serial_number = ui->lineEdit_ekanr->text();
    QString eka_certificate = ui->lineEdit_cert->text();
    QString eka_count_of_use = ui->label_number->text();
    QString eka_place_eka = ui->lineEdit_ekaplace->text();
    QString eka_c_name = ui->lineEdit_user->text();
    QString eka_eka_model = ui->comboBox_model->;

    dbm->insertClient(eka_serial_number, eka_certificate, eka_count_of_use, eka_place_eka, eka_c_name, eka_eka_model);
}
