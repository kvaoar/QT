#include "portselect.h"
#include "ui_portselect.h"

PortSelect::PortSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PortSelect)
{
    ui->setupUi(this);
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        if(!port.isBusy())
           ui->comboBox->addItem(port.systemLocation());


           //
       }

    setPortName(ui->comboBox->currentText());
    connect(ui->comboBox,SIGNAL(currentTextChanged(QString)), this, SLOT(setPortName(QString)) );
}

PortSelect::~PortSelect()
{
    delete ui;
}

void PortSelect::setPortName(QString s){ __port_name = s;}

QString PortSelect::port_name(){ return __port_name;}

