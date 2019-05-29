#include "console.h"
#include "ui_console.h"
#include<QDebug>

Console::Console(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);
    connect(ui->inText, SIGNAL(enterPressed(QString)),this, SIGNAL(send_cmd(QString)));
    connect(ui->inText, SIGNAL(tabPressed(QString)),this, SIGNAL(send_cmd(QString)));
}

Console::~Console()
{
    delete ui;
}

bool Console::getConnect_status() const
{
  return connect_status;
}

void Console::setSerial(SerialService *value)
{
  if(value != nullptr){
      serial = value;
      qDebug()<<"console"<<connect(this,SIGNAL(connect_statusChanged(bool)),serial,SLOT(setConnectState(bool))); //button on console bar control connection
      qDebug()<<"console"<<connect(serial,SIGNAL(RX_Line(QString)), this, SLOT(appendLine(QString))); //Rx line from serialservice to console out
      qDebug()<<"console"<<connect(serial,SIGNAL(tailChanged(QString)), this, SLOT(setTail(QString))); //Rx tail from serialservice to console out
      qDebug()<<"console"<<connect(this, SIGNAL(send_cmd(QString)),serial, SLOT(serial_send(QString)));
    }
}

void Console::setConnect_status(bool value)
{
    connect_status = value;
    emit connect_statusChanged(connect_status);
}

void Console::appendLine(QString s)
{
  ui->outText->append(s);
}

void Console::setStatus(QString s)
{
  ui->status_text->setText(s);
}

void Console::setTail(QString s)
{
  tail = s;
  if(s.length()<255)
  setStatus(s);
}

void Console::on_bManual_clicked(bool checked)
{

}
