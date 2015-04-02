#include "sqltablemodelro.h"

SqlTableModelRO::SqlTableModelRO(QObject *parent) :
    QSqlTableModel(parent)
{
}

Qt::ItemFlags SqlTableModelRO::flags(const QModelIndex &index) const
{

    /*Qt::ItemFlags result = QStandardItemModel::flags(index);
    if (index.column() == 0) //0 is the first column!
    {
        result &= ~Qt::ItemIsEditable;
    }*/
    Qt::ItemFlags result = QSqlTableModel::flags(index);
    result &= ~Qt::ItemIsEditable;
    return result;
}
