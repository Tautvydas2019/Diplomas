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

private:
    Ui::MainWindow *ui;
    DatabaseManager *dbm;
};

#endif // MAINWINDOW_H
