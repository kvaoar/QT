#include "uart.h"
#include "ui_uart.h"

UART::UART(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::UART)
{
  ui->setupUi(this);
}

UART::~UART()
{
  delete ui;
}

void UART::setSerial(SerialService *value)
{
  if(value != nullptr){
      serial = value;
      connect(this,&UART::sendStrCon,serial,&SerialService::serial_send);
    }
}

void UART::recieveLine(QString s)
{

}
