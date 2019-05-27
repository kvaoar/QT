#include "dac.h"
#include "ui_dac.h"

DAC::DAC(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DAC)
{
  ui->setupUi(this);
}

DAC::~DAC()
{
  delete ui;
}
