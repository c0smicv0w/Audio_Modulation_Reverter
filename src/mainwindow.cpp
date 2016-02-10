#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audiomgr.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);
    setControl();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setControl()
{
    AudioMgr::State state = mgr.state;
    ui->actionRecord->setEnabled(state == AudioMgr::Closed);
    ui->actionSuspend->setEnabled(state == AudioMgr::Active);
    ui->actionResume->setEnabled(state == AudioMgr::Suspended);
    ui->actionStop->setEnabled(state == AudioMgr::Active || state == AudioMgr::Suspended);
}

void MainWindow::on_actionRecord_triggered()
{
    mgr.initializeAudio();
    mgr.start();
    qDebug()<< "Record triggered";
    setControl();

}

void MainWindow::on_actionSuspend_triggered()
{
    qDebug() <<"Suspend triggered";
    mgr.suspend();
    setControl();
}

void MainWindow::on_actionStop_triggered()
{
    qDebug() <<"Stop triggered";
    mgr.stop();
    setControl();
}



void MainWindow::on_actionResume_triggered()
{
    qDebug() <<"Resume triggered";
    mgr.resume();
    setControl();
}