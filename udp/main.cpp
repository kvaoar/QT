#include "udp.h"
#include <QApplication>
#include "myudp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        MyUDP client;


         client.HelloUDP();

    MainWindow w;
    w.show();



    return a.exec();
}
