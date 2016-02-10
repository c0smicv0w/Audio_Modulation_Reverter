#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qSetMessagePattern("%{time yyyyMMdd h:mm:ss.zzz t} [%{file}:%{line}] %{threadid} %{message}");
    MainWindow w;
    w.show();

    return a.exec();
}
