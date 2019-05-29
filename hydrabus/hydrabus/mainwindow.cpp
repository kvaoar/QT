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

    ui->tabPWM->setSerial(serial);
    ui->tabSPI->setSerial(serial);
    ui->tabADC->setSerial(serial);
    ui->tabI2C->setSerial(serial);
    ui->tabDAC->setSerial(serial);
    ui->tabGPIO->setSerial(serial);
    ui->tabJTAG->setSerial(serial);
    ui->tabUART->setSerial(serial);
    ui->ConsoleWidget->setSerial(serial);

    qDebug()<<connect(serial,SIGNAL(connectChanged(bool)), this, SLOT(gui_connectionproc(bool))); // serialservise state show in gui
    ui->tabWidget->setEnabled(false);



}

MainWindow::~MainWindow()
{
    delete ui;
}


/*if(ui->bManual->isChecked()){   // Manual control mode, clean console
    QByteArray b = serialPort.readAll();
    buf += b;
    QStringList sl = buf.split("\r\n");
    if(sl.count()>1){
        for(int i = 0; i < sl.count()-1; i++) inputText<<sl[i]<<endl;
    }
    buf = sl[sl.count()-1]; //last string w/o \r\n

    if(buf.endsWith("> "))
        buf = "";   // command promt title
    else
    {
    if(buf.startsWith(' ')) buf.chop(1);
    if(buf.length()>0) ui->inText->cmdComplete(buf); // candidate to autocomplete
    }

    while (! inputText.atEnd()) {
        QString line = inputText.readLine();
        qDebug()<<line;
        ui->outText->appendPlainText(line);
    }
    ui->outText->moveCursor(QTextCursor::End);
}
else*/



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
                this, "hydrabus disconnect",
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


