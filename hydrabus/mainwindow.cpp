#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"portselect.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setFocusPolicy(Qt::NoFocus);
    ui->setupUi(this);

    serial = new SerialService(this);
    for(int i = 0; i <ui->tabWidget->count(); i++){
        WSerialClient* s = dynamic_cast<WSerialClient*>(ui->tabWidget->widget(i));

        if(s!= nullptr)s->setSerial(serial);
      }
    ui->ConsoleWidget->setSerial(serial);

    connect(serial,SIGNAL(connectChanged(bool)), this, SLOT(gui_connectionproc(bool))); // serialservise state show in gui
    ui->tabWidget->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnect_triggered()
{
 serial->setConnectState(true);
}


void MainWindow::on_actionDisconnect_triggered()
{
  serial->setConnectState(false);
}

void MainWindow::setTabIndex(int i)
{
  qDebug()<<i;
}


void MainWindow::gui_connectionproc(bool state)
{
    if(state){
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
         ui->tabWidget->setEnabled(true);
         ui->ConsoleWidget->setStatus("Connected.");
         serial->serial_send("\n");

    } else {
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
         ui->tabWidget->setEnabled(false);
         ui->ConsoleWidget->setStatus("Disconnected.");
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if(serial->connectState()){
    QMessageBox::StandardButton resBtn = QMessageBox::question(
                this, "disconnect",
                tr("Are you sure?\n"),
                QMessageBox::No | QMessageBox::Yes,
                QMessageBox::Yes);
       if (resBtn != QMessageBox::Yes) {
           event->ignore();
       } else {
           serial->setConnectState(false);
           event->accept();
       }
    }
}


