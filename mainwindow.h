#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QDataWidgetMapper>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *filesModel;
    QSqlTableModel *dl_logModel;
    QDataWidgetMapper *mapper;

private slots:
    void addFile();
    void changePath();
    void selectAnother(QModelIndex);
    //void generateLink();

};

#endif // MAINWINDOW_H
