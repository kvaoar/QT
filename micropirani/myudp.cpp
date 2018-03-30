#include "myudp.h"


MyUDP::MyUDP(QObject *parent) :
    QObject(parent)
{
    _is_connected = false;
   ESP_ip.append("192.168.1.1");
    socket = new QUdpSocket(this);
//socket->bind(QHostAddress(ESP_ip), 8888);
    connect(socket, SIGNAL(readyRead()), this, SLOT(udp_read()));
}

void MyUDP::udp_ask()
{
 if(_is_connected){
    QByteArray Data;
    Data.append("@253PR1?;FF");
    socket->writeDatagram(Data, QHostAddress(ESP_ip), 8888);
 }
}

void MyUDP::udp_read()
{
    QByteArray answer;
    QDateTime time = QDateTime::currentDateTime();
    answer.resize(socket->pendingDatagramSize());
    socket->readDatagram(answer.data(), answer.size(), &sender, &senderPort);
    emit udp_recieved(answer,time);
}



void MyUDP::udp_connect(bool state){
    _is_connected = state;
    emit is_connected( state);
}
