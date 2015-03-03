#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "naujasmodelis.h"
#include "ui_naujasmodelis.h"
#include "naujasklientas.h"
#include "ui_naujasklientas.h"
#include "settings.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dbm;
}

void MainWindow::on_actionNaujas_modelis_triggered()
{
    //Iškviečia Newmodel forma
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
    NaujasKlientas nKlientas(this);
    nKlientas.setModal(true);
    nKlientas.exec();
}
