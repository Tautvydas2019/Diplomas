#include "myqsqlrelationaltablemodel.h"

#include <QSqlRelationalTableModel>
#include <QModelIndex>
#include <Qt>
#include <QVariant>
#include <QDate>
#include <QColor>
#include <QBrush>

#include <QDebug>

QVariant MyQSqlRelationalTableModel::data(const QModelIndex & index, int role = Qt::DisplayRole) const {
    QVariant value = QSqlRelationalTableModel::data(index, role);

    if (role == Qt::TextColorRole) {
        int row = index.row();
        QModelIndex date_index = QSqlRelationalTableModel::index(row, 7);
        QDate checkup_date = data(date_index, Qt::DisplayRole).toDate();
        QDate current_date = QDate::currentDate();
        if (checkup_date <= current_date) {
            return QBrush(QColor(Qt::red));
        }
    }
    return value;
}

