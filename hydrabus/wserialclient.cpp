#include "wserialclient.h"
#include "wserialclient.h"
#include<QDebug>
WSerialClient::WSerialClient(QWidget *parent) : QWidget(parent)
{

}

void WSerialClient::setSerial(SerialService *value)
{
  if(value != nullptr){
       qDebug()<<"proto setSerial";
      serial = value;
      connect(this,&WSerialClient::sendStrCon,serial,&SerialService::serial_send);
    }
}

bool WSerialClient::getRx_en() const
{
    return rx_en;
}

void WSerialClient::setRx_en(bool value)
{
  if(rx_en != value){
  rx_en = value;
  if(rx_en) connect(serial,&SerialService::RX_Line,this,&WSerialClient::recieveLine);
  else disconnect(serial,&SerialService::RX_Line,this,&WSerialClient::recieveLine);
  emit recieveEnableChanged(rx_en);
  }
}

/*void WSerialClient::recieveLine(QString s)
{
    qDebug()<<"proto"<<s;
}*/

bool WSerialClient::getEnableRecieve()
{
    return rx_en;
}

