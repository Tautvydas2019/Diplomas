#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "databasemanager.h"

#include <QMainWindow>
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNaujas_modelis_triggered();

    void on_actionU_daryti_triggered();

    void on_actionNaujas_klientas_triggered();

    void on_actionNaujas_EKA_triggered();

    void on_actionKlient_duomen_baz_triggered();

    void on_actionEKA_duomen_baz_triggered();

    void on_pushButton_model_clicked();

    void on_pushButton_client_clicked();

    void on_pushButton_eka_clicked();

    void on_pushButton_clientedit_clicked();

    void on_pushButton_ekaedit_clicked();

private:
    Ui::MainWindow *ui;
    DatabaseManager *dbm;
};

#endif // MAINWINDOW_H
