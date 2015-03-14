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

private:
    QSqlRecord current_record;
    Ui::KlientuPaieska *ui;
    DatabaseManager *dbm;
    QSqlTableModel *table_model;
};

#endif // KLIENTUPAIESKA_H
