#include "jtag.h"
#include "ui_jtag.h"

JTAG::JTAG(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::JTAG)
{
  ui->setupUi(this);
}

JTAG::~JTAG()
{
  delete ui;
}

void JTAG::setSerial(SerialService *value)
{
  if(value != nullptr){
      serial = value;
      connect(this,&JTAG::sendStrCon,serial,&SerialService::serial_send);
    }
}

void JTAG::recieveLine(QString s)
{

}
