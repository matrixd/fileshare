#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "srvthread.h"
#include "server.h"
#include <QTableView>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QShortcut>

#include "mydb.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    filesModel = new QSqlTableModel(this);
    //model->setTable("settings");
    filesModel->setTable("files");
    filesModel->setEditStrategy(QSqlTableModel::OnRowChange);

    filesModel->select();

    QTableView *view = ui->filesView;
    view->setModel(filesModel);
    view->hideColumn(0); // don't show the ID
    view->hideColumn(filesModel->fieldIndex("path"));
    view->resizeColumnsToContents();
    view->show();

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(filesModel);
    mapper->addMapping(this->ui->enabledCheck, filesModel->fieldIndex("enabled"));
    mapper->addMapping(this->ui->publicCheck, filesModel->fieldIndex("public"));
    mapper->addMapping(this->ui->path, filesModel->fieldIndex("path"));
    mapper->toFirst();

    dl_logModel = new QSqlTableModel(this);
    dl_logModel->setTable("dl_log");
    dl_logModel->setEditStrategy(QSqlTableModel::OnRowChange);

    view = ui->dl_logView;
    view->setModel(dl_logModel);
    view->hideColumn(0); // don't show the ID
    view->resizeColumnsToContents();
    view->show();

    //SrvThread* thrd = new SrvThread(this);
    //thrd->start();

    Server *srv = new Server(this);
    srv->start();


    connect(this->ui->addButton, SIGNAL(pressed()), this, SLOT(addFile()));
    connect(this->ui->filesView, SIGNAL(clicked(QModelIndex)), mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    connect(this->ui->filesView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectAnother(QModelIndex)));
    connect(this->ui->pathButton, SIGNAL(pressed()), this, SLOT(changePath()));

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addFile()
{
    QString path = QFileDialog::getOpenFileName(this,
        tr("Select a file"), "~/", tr("Any files (*)"));

    qDebug() << path;

    if(path.count() < 1)
        return;

    QString fname = path.split('/').last();

    QString queryStr = "insert into files (path,fname,time) VALUES('%1', '%2', '%3')";
    queryStr = queryStr.arg(path,fname,MyDB::now());

    QSqlQuery query;
    query.exec(queryStr);
    qDebug() << queryStr;
    qDebug() << query.lastError().text();

    filesModel->select();
}

void MainWindow::changePath()
{
    QSqlRecord rec = filesModel->record(mapper->currentIndex());

    QString path = rec.value(filesModel->fieldIndex("path")).toString();

    path = QFileDialog::getOpenFileName(this,
        tr("Select a file"), path , tr("Any files (*)"));

    qDebug() << path;

    if(path.count() < 1)
        return;

    QString fname = path.split('/').last();

    QSqlQuery query;
    query.exec(QString("update files set path = '%1', time = '%2', fname = '%3' where id = %4").arg(path,MyDB::now(),fname,rec.value(0).toString()));

    filesModel->select();
}

void MainWindow::selectAnother(QModelIndex index)
{
    dl_logModel->setFilter(QString("file = %1").arg(index.row()));
    dl_logModel->select();

    QString link;

    //while debugging
    link = "http://localhost";

    link.append(':');
    link.append("3567");
    link.append('/');


    //const char c = index.row();
    //QByteArray ar(&c,1);

    //link.append(ar.toBase64(QByteArray::Base64UrlEncoding));

    //BUG!
    link.append(QString("%1").arg(index.row()));

    this->ui->linkEdit->setText(link);

    qDebug() << link;
}
