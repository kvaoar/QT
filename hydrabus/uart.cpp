#include "uart.h"
#include "ui_uart.h"

UART::UART(QWidget *parent) :
  WSerialClient(parent),
  ui(new Ui::UART)
{
  ui->setupUi(this);
}

UART::~UART()
{
  delete ui;
}


void UART::recieveLine(QString s)
{
Q_UNUSED(s);
}

void UART::on_bConnect_toggled(bool checked)
{
  if(checked){
      QString cmd = QStringLiteral("uart %1 speed %2 parity %3 %4\n").arg(ui->cDev->currentText()).arg(ui->cSpeed->currentText()).arg(ui->cParity->currentText()).arg(ui->cStopbit->currentText());
      emit sendStrCon(cmd);
    } else
    {
      QString cmd = "exit\n";
      emit sendStrCon(cmd);
    }
}

void UART::on_bPinout_clicked()
{
    emit sendStrCon("show pin\n");
}

void UART::on_bWrite_clicked()
{
  QString cmd = QStringLiteral("%1\n").arg(ui->eWrite->text());
    emit sendStrCon(cmd);
}

void UART::on_bRead_clicked()
{
  QString cmd = QStringLiteral("%1\n").arg(ui->eRead->text());
    emit sendStrCon(cmd);
}
