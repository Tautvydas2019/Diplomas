#ifndef EKAEDITING_H
#define EKAEDITING_H

#include "databasemanager.h"
#include "klientupaieska.h"
#include "myqsqlrelationaltablemodel.h"

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QWidget>

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

    void on_lineEdit_search_textChanged(const QString &filter);

    void on_tableView_activated(const QModelIndex &index);

    void on_pushButton_delete_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_atsaukti_clicked();

    void setClient();

    void on_toolButton_client2_clicked();

private:
    Ui::EkaEditing *ui;
    DatabaseManager *dbm;
    MyQSqlRelationalTableModel *table_model;
    QSqlQueryModel *client_info_model;
    QSqlRecord current_record;
    KlientuPaieska *client_search_dialog;
    QSqlRecord client_record;
    bool isEditButton;

    void updateTextEdits(const QModelIndex &index);
};

#endif // EKAEDITING_H
