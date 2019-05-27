#include "iinterface.h"
#include "ui_iinterface.h"

IInterface::IInterface(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::IInterface)
{
  ui->setupUi(this);
}

IInterface::~IInterface()
{
  delete ui;
}
