#ifndef MYDB_H
#define MYDB_H

#include <QString>

class MyDB
{
public:
    MyDB();
    static bool init();
    static QString now();
    static QByteArray getSettings(QString option);
    static void updSettings(QString option, QString value);

private:
     static void create();

};

#endif // MYDB_H
