#include "dac.h"
#include "ui_dac.h"

DAC::DAC(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DAC)
{
  ui->setupUi(this);
  ui->sVolt->setVisible(false);
  ui->sVolt_2->setVisible(false);
}

DAC::~DAC()
{
  delete ui;
}

void DAC::setSerial(SerialService *value)
{
  if(value != nullptr){
      serial = value;
      connect(this,
              &DAC::sendStrCon,
              serial,
              &SerialService::serial_send);
    }
}

void DAC::recieveLine(QString s)
{

}

void DAC::sw_mod1(int i)
{
  if(i == 0) { ui->sRaw->setVisible(true); ui->sVolt->setVisible(false); }
  if(i == 1) { ui->sRaw->setVisible(false); ui->sVolt->setVisible(true); }

}

void DAC::sw_mod2(int i)
{
  if(i == 0) { ui->sRaw_2->setVisible(true); ui->sVolt_2->setVisible(false); }
  if(i == 1) { ui->sRaw_2->setVisible(false); ui->sVolt_2->setVisible(true); }

}

void DAC::cmd(int dev)
{
  QString devname = QStringLiteral("dac%1").arg(dev);
  bool to_connect = (dev == 1)?(ui->bStart->isChecked()):(ui->bStart_2->isChecked());
  if(to_connect){
      QString val = (dev == 1)?(ui->cMode->currentText()):(ui->cMode_2->currentText());
      if(dev == 1){
      if(ui->cMode->currentIndex() == 0) val = QStringLiteral("%1 %2").arg(val).arg(ui->sRaw->value());
      if(ui->cMode->currentIndex() == 1) val = QStringLiteral("%1 %2").arg(val).arg(ui->sVolt->value());
        }
      if (dev == 2) {
          if(ui->cMode_2->currentIndex() == 0) val = QStringLiteral("%1 %2").arg(val).arg(ui->sRaw_2->value());
          if(ui->cMode_2->currentIndex() == 1) val = QStringLiteral("%1 %2").arg(val).arg(ui->sVolt_2->value());
}
      QString cmd = QStringLiteral("dac %1 %2\n").arg(devname).arg(val);
      emit sendStrCon(cmd);
    } else {
      QString cmd = QStringLiteral("dac %1 exit\n").arg(devname);
      emit sendStrCon(cmd);
  }
}

void DAC::connect1(bool st)
{
  cmd(1);
}

void DAC::connect2(bool st)
{
  cmd(2);
}


