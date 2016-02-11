#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include "audiomgr.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);
    setControl();

    connect(&mgr, SIGNAL(dataAvail(AudioDataParam)), SLOT(draw(AudioDataParam)));
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

void MainWindow::draw(AudioDataParam param)
{
    ui->pcmWidget->clear();
    ui->pcmWidget->add(param.pcmIn, QPen(Qt::white));
    ui->pcmWidget->add(param.pcmOut, QPen(Qt::red));
    ui->pcmWidget->repaint();

    ui->freqWidget->clear();
    ui->freqWidget->add(param.freqIn, QPen(Qt::white));
    ui->freqWidget->add(param.freqOut, QPen(Qt::red));
    ui->freqWidget->repaint();
}
