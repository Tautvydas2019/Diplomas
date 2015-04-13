#ifndef IMPORTDBF_H
#define IMPORTDBF_H

#include "databasemanager.h"
#include "qdbftablemodel.h"

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class ImportDbf;
}

class ImportDbf : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDbf(QWidget *parent, DatabaseManager *dbm);
    ~ImportDbf();

private slots:
    void on_toolButton_eka_clicked();

    void on_toolButton_client_clicked();

    void on_toolButton_model_clicked();

    void on_pushButton_eka_clicked();

    void on_pushButton_client_clicked();

    void on_pushButton_model_clicked();

    void on_pushButton_addToDb_clicked();

private:
    Ui::ImportDbf *ui;
    DatabaseManager *dbm;
    QSqlTableModel *client_model;
    QSqlTableModel *eka_model;
    QSqlTableModel *model_model;
    QDbfTable client_table;
    QDbfTable model_table;
    QDbfTable eka_table;
};

#endif // IMPORTDBF_H
