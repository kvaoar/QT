#include "spi.h"
#include "ui_spi.h"

#include<QDebug>

SPI::SPI(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SPI)
{

  ui->setupUi(this);
  ui->SPI_frame->setEnabled(false);
}

SPI::~SPI()
{
  delete ui;
}

bool SPI::getEnableRecieve()
{
  return rx_en;
}

void SPI::setSerial(SerialService *value)
{
  if(value != nullptr){
      serial = value;
      qDebug()<<"SPI"<<connect(this,&SPI::sendStrCon,serial,&SerialService::serial_send);
    }
}

void SPI::recieveLine(QString s)
{

}

void SPI::setSPI(bool st)
{
  if(spi_on != st){
      if(st)
        spi_connect();
        else
        spi_disconnect();
      spi_on = st;
    }
}

void SPI::setDevice(int index)
{

  if(index == 0){
      ui->cSPI_freq->clear();
      ui->cSPI_freq->addItems(dev1);
  }else{
      ui->cSPI_freq->clear();
      ui->cSPI_freq->addItems(dev2);
  }

  ui->cSPI_freq->setCurrentIndex(0);
}

void SPI::cmdRead()
{
  QString cmd = "r\n";
  if(ui->eSPI_singleRead->text().length() == 0)
      emit sendStrCon(cmd);
  else {
      emit sendStrCon(ui->eSPI_singleRead->text()+"\n");
  }
}

void SPI::cmdWrite()
{
emit sendStrCon(ui->eSPI_singleWrite->text()+"\n");
}

void SPI::cmdPinout()
{

}

void SPI::cmdNSS(bool st)
{
  if(st)
      emit sendStrCon("[\n");
  else {
      emit sendStrCon("]\n");
  }
}

void SPI::cmdDump()
{
  enum {NO=0, FULL=1, RX=2, TX=3, RXTXGAP=4};
  QString addr = ui->eSPI_DumpAddr->text();
  int len = ui->eSPI_DumpLen->value();
  int nss_mode = ui->eSPI_NSSmode->currentIndex();
  QString mcmd = "";
  if(len<255){
      mcmd = "r:";
  } else{
      mcmd = "hd:";
  }

  QString command = "";
  switch (nss_mode) {
  case NO:
      command = addr+" "+mcmd+ QStringLiteral("%1\n").arg(len);
      break;
  case FULL:
      command = "[ "+addr+" "+mcmd+ QStringLiteral("%1").arg(len)+" ]\n";
      break;
  case RX:
      command = addr+" [ "+mcmd+ QStringLiteral("%1").arg(len)+" ]\n";
      break;
  case TX:
      command = "[ "+addr+" ] "+mcmd+ QStringLiteral("%1").arg(len)+"\n";
      break;
  case RXTXGAP:
      command = "[ "+addr+" ] % [ "+mcmd+ QStringLiteral("%1").arg(len)+" ]\n";
      break;
  default:
      break;
  }

  ui->eSPI_DumpCmdOut->setText(command);
  emit sendStrCon(command);
}

void SPI::spi_connect()
{
  QString cmd = QStringLiteral("spi %1 pull %2 mode %3 frequency %4 %5 %6\n").arg(ui->cSPI_dev->currentText())\
      .arg(ui->cSPI_pull->currentText()).arg(ui->cSPI_role->currentText()).arg(ui->cSPI_freq->currentText())\
      .arg(ui->cSPI_polph->currentText()).arg(ui->cSPI_msb->currentText());
ui->SPI_frame->setEnabled(true);
emit sendStrCon(cmd);
}

void SPI::spi_disconnect()
{
  ui->SPI_frame->setEnabled(false);
  emit sendStrCon("exit\n");
}

