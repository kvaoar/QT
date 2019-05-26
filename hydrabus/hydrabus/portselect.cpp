#include "portselect.h"
#include "ui_portselect.h"

PortSelect::PortSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PortSelect)
{
    ui->setupUi(this);
}

PortSelect::~PortSelect()
{
    delete ui;
}
