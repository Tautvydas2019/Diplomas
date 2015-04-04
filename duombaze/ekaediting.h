#ifndef EKAEDITING_H
#define EKAEDITING_H

#include "databasemanager.h"
#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>

namespace Ui {
class EkaEditing;
}

class EkaEditing : public QDialog
{
    Q_OBJECT

public:
    explicit EkaEditing(QWidget *parent, DatabaseManager *dbm);
    ~EkaEditing();

private slots:



    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::EkaEditing *ui;
    DatabaseManager *dbm;
    QSqlTableModel *table_model;
};

#endif // EKAEDITING_H
