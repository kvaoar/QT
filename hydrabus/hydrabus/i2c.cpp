#include "i2c.h"
#include "ui_i2c.h"

#include<QDebug>
I2C::I2C(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::I2C)
{
  ui->setupUi(this);
  ui->pcmd->setEnabled(false);
  ui->ptransact->setEnabled(false);
}

I2C::~I2C()
{
  delete ui;
}

void I2C::setSerial(SerialService *value)
{
  if(value != nullptr){
      serial = value;
      qDebug()<<"I2C"<<connect(this,&I2C::sendStrCon,serial,&SerialService::serial_send);
    }
}

void I2C::recieveLine(QString s)
{

}

void I2C::connect_i2c(bool st)
{
  if(st){
      emit sendStrCon(QStringLiteral("i2c pull %1 frequency %2\n").arg(ui->cPull->currentText()).arg(ui->cFreq->currentText()));
    }else{
      emit sendStrCon("exit\n");
    }
  ui->pcmd->setEnabled(st);
  ui->ptransact->setEnabled(st);
}

void I2C::scan_cmd()
{
  emit sendStrCon("scan\n");
}

void I2C::read_cmd()
{

  emit sendStrCon(QStringLiteral("read %1\n").arg(ui->eRead->text()));
}

void I2C::write_cmd()
{
  emit sendStrCon(QStringLiteral("write %1\n").arg(ui->eWrite->text()));
}

void I2C::transact_cmd()
{
  QString cmd = "";

      if(ui->sDataCnt->value() > 1)
        cmd = QStringLiteral("%1 read:%2 %3\n").arg(ui->eAddr->text()).arg(ui->sDataCnt->value()).arg(ui->eData->text());
      else
        cmd = QStringLiteral("%1 read %3\n").arg(ui->eAddr->text()).arg(ui->eData->text());

  if(ui->cStartStop) cmd = "[ "+ cmd + " ]";
ui->eTrCmd->setText(cmd);
  emit sendStrCon(cmd+"\n");
}

void I2C::startstop(bool st)
{
  if(st)
    emit sendStrCon("[\n");
  else
    emit sendStrCon("]\n");
}
