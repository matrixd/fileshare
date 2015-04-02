#ifndef MYDB_H
#define MYDB_H

class MyDB
{
public:
    MyDB();
    static bool init();

private:
     static void create();

};

#endif // MYDB_H
