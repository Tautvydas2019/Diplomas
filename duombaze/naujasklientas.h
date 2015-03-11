#ifndef NAUJASKLIENTAS_H
#define NAUJASKLIENTAS_H

#include "databasemanager.h"

#include <QDialog>
#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class NaujasKlientas;
}

class NaujasKlientas : public QDialog
{
    Q_OBJECT

public:
    explicit NaujasKlientas(QWidget *parent, DatabaseManager *dbm);
    ~NaujasKlientas();

private slots:
    void on_pushButton_clicked();

private:
    DatabaseManager *dbm;
    Ui::NaujasKlientas *ui;
    QSqlTableModel *table_model;
};

#endif // NAUJASKLIENTAS_H
