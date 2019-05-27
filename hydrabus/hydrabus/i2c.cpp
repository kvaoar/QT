#include "i2c.h"
#include "ui_i2c.h"

I2C::I2C(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::I2C)
{
  ui->setupUi(this);
}

I2C::~I2C()
{
  delete ui;
}
