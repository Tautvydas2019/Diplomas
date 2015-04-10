#ifndef IMPORTDBF_H
#define IMPORTDBF_H

#include "databasemanager.h"
#include "qdbftablemodel.h"

#include <QDialog>

namespace Ui {
class ImportDbf;
}

class ImportDbf : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDbf(QWidget *parent, DatabaseManager *dbm);
    ~ImportDbf();

private slots:
    void on_toolButton_eka_clicked();

    void on_toolButton_client_clicked();

private:
    Ui::ImportDbf *ui;
    DatabaseManager *dbm;
    QDbf::QDbfTableModel *dbf_client_model;
    QDbf::QDbfTableModel *dbf_eka_model;
};

#endif // IMPORTDBF_H
