#include "mydb.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>
#include <QDebug>
#include <QSqlError>

MyDB::MyDB()
{

}

bool MyDB::init()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("main.db");
    if (!db.open()) {
        /*QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "This example needs SQLite support. Please read "
                                       "the Qt SQL driver documentation for information how "
                                       "to build it.\n\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);*/
        return 0;
    }

    QStringList tables = db.tables();
    if (tables.contains("settings", Qt::CaseInsensitive)
            && tables.contains("files", Qt::CaseInsensitive)
            && tables.contains("dl_log", Qt::CaseInsensitive))
        return 1;

    create();

    return 1;
}

void MyDB::create()
{
    QSqlQuery query;
    //files table
    query.exec("create table files (id integer primary key autoincrement not null, "
               "hash text unique not null, "
               "path text not null,"
               "fname text not null, "
               "public integer not null default(0), "
               "enabled integer not null default(1))");
    qDebug() << query.lastError().text();

    //download log table
    query.exec("create table dl_log (id integer primary key autoincrement not null, "
               "file int not null, "
               "ip text not null, "
               "client text not null, "
               "time text not null)");
    qDebug() << query.lastError().text();

    //settings table
    query.exec("create table settings (name text primary key not null, "
               "value blob)");
    qDebug() << query.lastError().text();

    //insert settings
    query.exec("insert into settings values ('port','3567')");
    qDebug() << query.lastError().text();

}

