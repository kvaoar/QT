#include "pwm.h"
#include "ui_pwm.h"
#include<QDebug>

PWM::PWM(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PWM)
{
  ui->setupUi(this);
  ramp_tim = new QTimer();
  ramp_tim->setTimerType(Qt::TimerType::PreciseTimer);
  qDebug()<<"pwm"<<connect(ramp_tim,SIGNAL(timeout()),this,SLOT(tim()));
}

PWM::~PWM()
{
  delete ui;
}

WorkState PWM::getStatus() const
{
  return status;
}

void PWM::setSerial(SerialService *value)
{
  if(value != nullptr){
      serial = value;
      connect(this,&PWM::sendStrCon,serial,&SerialService::serial_send);
    }
}

void PWM::setStatus(const WorkState &value)
{
  if(value != status){
  if(status == DIAL_RTCONTROL) DialDeInit();
  if(status == RAMP_OUT) RampDeInit();
  //status == NOP here
  if(value == DIAL_RTCONTROL) {DialInit();}
  if(value == RAMP_OUT) {RampInit();}
  status = value;
    }
}

QString PWM::cmd_compiler(int freq, int duty)
{
  return QStringLiteral("pwm frequency %1 duty-cycle %2\n").arg(freq).arg(duty);
}

void PWM::RampInit()
{
  ui->bdialSend->setEnabled(false);
  ui->dial_Freq->setEnabled(false);
  ui->sFreq->setEnabled(false);
  ramp_freq = ui->sPWM_startF->value();
  ramp_tim->start(ui->sPWM_TRamp->value());
}

void PWM::RampDeInit()
{
  ui->bdialSend->setEnabled(true);
  ui->dial_Freq->setEnabled(true);
  ui->sFreq->setEnabled(true);
  ramp_tim->stop();
}

void PWM::DialInit()
{
  ui->bPWM_Ramp->setEnabled(false);
}

void PWM::DialDeInit()
{
  ui->bPWM_Ramp->setEnabled(true);
}

void PWM::recieveLine(QString s)
{

}

bool PWM::getEnableRecieve()
{
  return false;
}


void PWM::guiChange(int val)
{
  Q_UNUSED(val);
  qDebug()<<"chan"<<val;
  if(getStatus() == DIAL_RTCONTROL){
      sendStrCon(cmd_compiler(ui->sFreq->value(),ui->sDirty->value()));
    }
  if(getStatus() == RAMP_OUT){
      if(ramp_tim->interval() != ui->sPWM_TRamp->value()) ramp_tim->start(ui->sPWM_TRamp->value());
    }

}

void PWM::guiRamp(bool on)
{
  qDebug()<<"ramp";
  if(on)
    setStatus(WorkState::RAMP_OUT);
  else
    setStatus(WorkState::NO_STATE);
}

void PWM::guiDial(bool on)
{
  if(on)
    setStatus(WorkState::DIAL_RTCONTROL);
  else
    setStatus(WorkState::NO_STATE);
}

void PWM::guiSend()
{
emit sendStrCon(cmd_compiler(ui->sFreq->value(),ui->sDirty->value()));
}

void PWM::tim()
{
ramp_freq += ui->sPWM_FStep->value();
if(ramp_freq > ui->sPWM_stopF->value()) ramp_freq = ui->sPWM_startF->value();
ui->sFreq->setValue(ramp_freq);
sendStrCon(cmd_compiler(ui->sFreq->value(),ui->sDirty->value()));
}
