#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h"
#include <QTableView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new SqlTableModelRO(this);
    model->setTable("settings");
    //model->setTable("files");
    model->setEditStrategy(QSqlTableModel::OnRowChange);

    model->select();

    QTableView *view = ui->tableView;
    view->setModel(model);
    view->hideColumn(0); // don't show the ID
    view->show();

    Server* srv = new Server();
    srv->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
