#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  ui->pcmWidget->clear();

  QByteArray pcm;
  for (int i = 0; i < 44100; i++) {
    short s;
    s = (i * 100) % 32768;
    unsigned char ch1 = (s & 0x00FF);
    unsigned char ch2 = (s & 0xFF00) >> 8;
    pcm.append(ch1);
    pcm.append(ch2);
  }
  ui->pcmWidget->add(pcm, QPen(Qt::white));

  pcm.clear();
  for (int i = 0; i < 44100; i++) {
    short s;
    s = (i * 150) % 32768;
    unsigned char ch1 = (s & 0x00FF);
    unsigned char ch2 = (s & 0xFF00) >> 8;
    pcm.append(ch1);
    pcm.append(ch2);
  }
  ui->pcmWidget->add(pcm, QPen(Qt::gray));

  ui->pcmWidget->repaint();
}
