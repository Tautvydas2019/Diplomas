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

    void on_lineEdit_search_textChanged(const QString &filter);

    void on_tableView_activated(const QModelIndex &index);

    void on_pushButton_delete_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_atsaukti_clicked();

private:
    Ui::EkaEditing *ui;
    DatabaseManager *dbm;
    QSqlTableModel *table_model;
    QSqlRecord current_record;
    bool isEditButton;

    void updateTextEdits(const QModelIndex &index);
};

#endif // EKAEDITING_H
