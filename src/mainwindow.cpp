#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audiomgr.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRecord_triggered()
{
    mgr.initializeAudio();
    mgr.start();
    qDebug()<< "Record triggered";

}

void MainWindow::on_actionSuspend_triggered()
{
    qDebug() <<"Suspend triggered";

}

void MainWindow::on_actionStop_triggered()
{
    qDebug() <<"Stop triggered";
}


