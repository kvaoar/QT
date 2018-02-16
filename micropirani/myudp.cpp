




















// myudp.cpp

#include "myudp.h"

MyUDP::MyUDP(QObject *parent) :
    QObject(parent)
{

   ESP_ip.append("192.168.1.1");

    socket = new QUdpSocket(this);

    socket->bind(QHostAddress(ESP_ip), 8888);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void MyUDP::HelloUDP()
{

    QByteArray Data;
    Data.append("@253PR1?;FF");


    socket->writeDatagram(Data, QHostAddress(ESP_ip), 8888);
}

void MyUDP::readyRead()
{

    QByteArray answ;
    answ.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;


    socket->readDatagram(answ.data(), answ.size(),
                         &sender, &senderPort);

    qDebug() << "ESP8266_Addr: " << sender.toString();
    qDebug() << "ESP8266_Port: " << senderPort;
    qDebug() << "Answer  " << answ;
}

