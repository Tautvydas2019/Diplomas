#ifndef MYQSQLRELATIONALTABLEMODEL_H
#define MYQSQLRELATIONALTABLEMODEL_H

#include <QSqlRelationalTableModel>
#include <QModelIndex>
#include <Qt>
#include <QVariant>

class MyQSqlRelationalTableModel : public QSqlRelationalTableModel
{
public:

    using QSqlRelationalTableModel::QSqlRelationalTableModel;
public:
    QVariant data(const QModelIndex & index, int role) const;
};

#endif // MYQSQLRELATIONALTABLEMODEL_H
