#ifndef KLIENTUPAIESKA_H
#define KLIENTUPAIESKA_H

#include <QDialog>
#include "databasemanager.h"
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>

namespace Ui {
class KlientuPaieska;
}

class KlientuPaieska : public QDialog
{
    Q_OBJECT

public:
    explicit KlientuPaieska(QWidget *parent, DatabaseManager *dbm);
    ~KlientuPaieska();

    QSqlRecord getCurrentClient();

private slots:
    void on_lpaieska_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_checkBox_stateChanged(int arg1);

private:

    QSqlRecord current_record;
    QSqlRecord current_record_eka;
    Ui::KlientuPaieska *ui;
    DatabaseManager *dbm;
    QSqlTableModel *table_model;
    QSqlRelationalTableModel *table_model_eka;
};

#endif // KLIENTUPAIESKA_H
