#include "spi.h"
#include "ui_spi.h"

SPI::SPI(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SPI)
{
  ui->setupUi(this);
}

SPI::~SPI()
{
  delete ui;
}
