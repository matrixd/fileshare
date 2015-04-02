#ifndef SQLTABLEMODELRO_H
#define SQLTABLEMODELRO_H

#include <QSqlTableModel>

class SqlTableModelRO : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit SqlTableModelRO(QObject *parent = 0);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

signals:

public slots:

};

#endif // SQLTABLEMODELRO_H
