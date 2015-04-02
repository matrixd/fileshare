#include "logviewer.h"
#include "ui_logviewer.h"

LogViewer::LogViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogViewer)
{
    ui->setupUi(this);
}

LogViewer::~LogViewer()
{
    delete ui;
}
