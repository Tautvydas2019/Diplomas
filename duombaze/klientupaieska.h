#ifndef KLIENTUPAIESKA_H
#define KLIENTUPAIESKA_H

#include <QDialog>
#include "databasemanager.h"
#include <QSqlTableModel>
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
    void on_tableView_activated(const QModelIndex &index);

    void on_lpaieska_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:

    QSqlRecord current_record;
    Ui::KlientuPaieska *ui;
    DatabaseManager *dbm;
    QSqlTableModel *table_model;
    QSqlTableModel *table_model_eka;
};

#endif // KLIENTUPAIESKA_H
