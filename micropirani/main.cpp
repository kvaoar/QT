#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <qlogging.h>

int main(int argc, char *argv[])
{
    qDebug()<<"run";
    qSetMessagePattern("%{file}(%{line}): %{message}");
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
