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

    void on_radioButton_gar_toggled(bool checked);

    void on_radioButton_nuom_toggled(bool checked);

private:

    Ui::NaujasEka *ui;
    DatabaseManager *dbm;
    QSqlTableModel *models_model;
    QSqlTableModel *eka_model;
    KlientuPaieska *client_search_dialog;
    QSqlRecord client_record;
};

#endif // NAUJASEKA_H
