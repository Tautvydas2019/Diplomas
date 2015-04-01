#ifndef NAUJASEKA_H
#define NAUJASEKA_H

#include "databasemanager.h"
#include "klientupaieska.h"

#include <QDialog>
#include <QWidget>
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

    void setClient();

    void on_toolButton_clicked();

    void on_checkBox_gar_stateChanged(int arg1);

    void on_checkBox_nuom_stateChanged(int arg1);

    void on_checkBox_place_stateChanged(int arg1);

private:

    Ui::NaujasEka *ui;
    DatabaseManager *dbm;
    QSqlTableModel *models_model;
    QSqlTableModel *eka_model;
    KlientuPaieska *client_search_dialog;
    QSqlRecord client_record;
};

#endif // NAUJASEKA_H
