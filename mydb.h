#ifndef MYDB_H
#define MYDB_H

#include <QString>

class MyDB
{
public:
    MyDB();
    static bool init();
    static QString now();

private:
     static void create();

};

#endif // MYDB_H
