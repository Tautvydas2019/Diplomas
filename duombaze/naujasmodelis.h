#ifndef NAUJASMODELIS_H
#define NAUJASMODELIS_H

#include "databasemanager.h"
#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class NaujasModelis;
}

class NaujasModelis : public QDialog
{
    Q_OBJECT

public:
    explicit NaujasModelis(QWidget *parent, DatabaseManager *dbm);
    ~NaujasModelis();

private slots:
    void on_pushButton_clicked();

private:
    DatabaseManager *dbm;
    Ui::NaujasModelis *ui;
    QSqlTableModel *table_model;
};

#endif // NAUJASMODELIS_H
