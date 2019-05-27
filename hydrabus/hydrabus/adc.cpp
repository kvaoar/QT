#include "adc.h"
#include "ui_adc.h"

ADC::ADC(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ADC)
{
  ui->setupUi(this);
}

ADC::~ADC()
{
  delete ui;
}
