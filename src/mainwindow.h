#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "audiomgr.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    AudioMgr mgr;
    void setControl();

public slots:
    void draw(AudioDataParam param);


private slots:
    void on_actionRecord_triggered();
    void on_actionSuspend_triggered();
    void on_actionStop_triggered();
    void on_actionResume_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
