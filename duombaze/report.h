#ifndef REPORT_H
#define REPORT_H

#include <QDialog>
#include <QSqlTableModel>

#include "DatabaseManager.h"

namespace Ui {
class Report;
}

class Report : public QDialog
{
    Q_OBJECT

public:
    explicit Report(QWidget *parent, DatabaseManager *dbm);
    ~Report();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::Report *ui;
    DatabaseManager *dbm;
    QSqlTableModel *model_model;
};

#endif // REPORT_H
