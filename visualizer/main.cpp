#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug()<<"we started";
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
