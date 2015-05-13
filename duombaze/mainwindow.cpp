#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "naujasmodelis.h"
#include "ui_naujasmodelis.h"
#include "naujasklientas.h"
#include "ui_naujasklientas.h"
#include "settings.h"
#include "naujaseka.h"
#include "ui_naujaseka.h"
#include "klientupaieska.h"
#include "ui_klientupaieska.h"
#include "ekaediting.h"
#include "ui_ekaediting.h"
#include "importdbf.h"
#include "report.h"

#include <QWidget>
#include <QMainWindow>
#include <QString>

#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget::setWindowTitle(Settings::TEXT_MAINWINDOW_NAME);
    dbm = new DatabaseManager();
    dbm->init();

    ui->pushButton->hide();
}
MainWindow::~MainWindow()
{
    delete ui;
    delete dbm;
}

void MainWindow::on_actionNaujas_modelis_triggered()
{
    //Iškviečia Naujasmodelis forma
    NaujasModelis nModel(this, dbm);
    nModel.setModal(true);
    nModel.exec();
}

void MainWindow::on_actionU_daryti_triggered()
{
    //Uždaryti programa
    close();
}

void MainWindow::on_actionNaujas_klientas_triggered()
{
    //Iškviečia NaujasKlientas forma
    NaujasKlientas nKlientas(this, dbm);
    nKlientas.setModal(true);
    nKlientas.exec();
}

void MainWindow::on_actionNaujas_EKA_triggered()
{
    NaujasEka nEka(this, dbm);
    nEka.setModal(true);
    nEka.exec();
}

void MainWindow::on_actionKlient_duomen_baz_triggered()
{
    KlientuPaieska nPaieska(this, dbm);
    nPaieska.setModal(true);
    nPaieska.exec();
}

void MainWindow::on_actionEKA_duomen_baz_triggered()
{
    EkaEditing nEkaEditing(this, dbm);
    nEkaEditing.setModal(true);
    nEkaEditing.exec();
}


void MainWindow::on_pushButton_model_clicked()
{
    NaujasModelis nModel(this, dbm);
    nModel.setModal(true);
    nModel.exec();
}

void MainWindow::on_pushButton_client_clicked()
{
    NaujasKlientas nKlientas(this, dbm);
    nKlientas.setModal(true);
    nKlientas.exec();
}

void MainWindow::on_pushButton_eka_clicked()
{
    NaujasEka nEka(this, dbm);
    nEka.setModal(true);
    nEka.exec();
}

void MainWindow::on_pushButton_clientedit_clicked()
{
    KlientuPaieska nPaieska(this, dbm);
    nPaieska.setModal(true);
    nPaieska.exec();
}

void MainWindow::on_pushButton_ekaedit_clicked()
{
    EkaEditing nEkaEditing(this, dbm);
    nEkaEditing.setModal(true);
    nEkaEditing.exec();
}

void MainWindow::on_pushButton_clicked()
{
    ImportDbf nImportDbf(this, dbm);
    nImportDbf.setModal(true);
    nImportDbf.exec();
}

void MainWindow::on_pushButton_3_clicked()
{
    Report nReport(this, dbm);
    nReport.setModal(true);
    nReport.exec();
}

void MainWindow::on_actionAtaskaitos_triggered()
{
    Report nReport(this, dbm);
    nReport.setModal(true);
    nReport.exec();
}
