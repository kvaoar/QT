#include "uart.h"
#include "ui_uart.h"

UART::UART(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::UART)
{
  ui->setupUi(this);
}

UART::~UART()
{
  delete ui;
}
