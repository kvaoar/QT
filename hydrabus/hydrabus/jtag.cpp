#include "jtag.h"
#include "ui_jtag.h"

JTAG::JTAG(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::JTAG)
{
  ui->setupUi(this);
}

JTAG::~JTAG()
{
  delete ui;
}
