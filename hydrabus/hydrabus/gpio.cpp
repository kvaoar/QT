#include "gpio.h"
#include "ui_gpio.h"

GPIO::GPIO(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::GPIO)
{
  ui->setupUi(this);
}

GPIO::~GPIO()
{
  delete ui;
}
