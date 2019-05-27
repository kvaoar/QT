#include "pwm.h"
#include "ui_pwm.h"

PWM::PWM(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PWM)
{
  ui->setupUi(this);
}

PWM::~PWM()
{
  delete ui;
}
