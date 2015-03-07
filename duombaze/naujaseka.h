#ifndef NAUJASEKA_H
#define NAUJASEKA_H

#include <QDialog>
#include "databasemanager.h"
#include <QSqlTableModel>

namespace Ui {
class NaujasEka;
}

class NaujasEka : public QDialog
{
    Q_OBJECT

public:
    explicit NaujasEka(QWidget *parent, DatabaseManager *dbm);
    ~NaujasEka();

private slots:
    void on_pushButton_save_clicked();

private:
    Ui::NaujasEka *ui;
    DatabaseManager *dbm;
    QSqlTableModel *models_model;
    QSqlTableModel *eka_model;
};

#endif // NAUJASEKA_H
