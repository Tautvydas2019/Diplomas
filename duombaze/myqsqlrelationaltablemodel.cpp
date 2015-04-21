#include "myqsqlrelationaltablemodel.h"

#include <QSqlRelationalTableModel>
#include <QModelIndex>
#include <Qt>
#include <QVariant>
#include <QDate>
#include <QColor>
#include <QBrush>

#include <QDebug>

QVariant MyQSqlRelationalTableModel::data(const QModelIndex & index, int role) const {
    QVariant value = QSqlRelationalTableModel::data(index, role);

    if (role == Qt::TextColorRole) {
        int row = index.row();
        QModelIndex date_index = QSqlRelationalTableModel::index(row, 7);
        QModelIndex status_index = QSqlRelationalTableModel::index(row, 11);
        QString status = data(status_index, Qt::DisplayRole).toString();
        QDate checkup_date = data(date_index, Qt::DisplayRole).toDate();
        QDate current_date = QDate::currentDate();
        if (status == "Nedirba") {
            return QBrush(QColor(Qt::blue));
        }
        if (checkup_date <= current_date) {
            return QBrush(QColor(Qt::red));
        }

    }
    return value;
}

