#include "gpio.h"
#include "ui_gpio.h"

GPIO::GPIO(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::GPIO)
{
  ui->setupUi(this);
}

GPIO::~GPIO()
{
  delete ui;
}

void GPIO::setSerial(SerialService *value)
{
  if(value != nullptr){
      serial = value;
      connect(this,&GPIO::sendStrCon,serial,&SerialService::serial_send);
    }
}

void GPIO::recieveLine(QString s){

};
