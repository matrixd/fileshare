#include "mainwindow.h"
#include <QApplication>
#include "mydb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MyDB::init();

    MainWindow w;
    w.show();

    return a.exec();
}
