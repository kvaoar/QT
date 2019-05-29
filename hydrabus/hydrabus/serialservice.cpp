#include "serialservice.h"
#include<QDebug>
#include<QFileInfo>

SerialService::SerialService(QObject *parent) : QObject(parent)
{
  rx_lines = new QStringList();
  rx_tim = new QTimer();
  rx_tim->setSingleShot(true);
  rx_tim->setInterval(200);
  rx_tim->setTimerType(Qt::TimerType::PreciseTimer);
  connect(rx_tim,SIGNAL(timeout()),this,SLOT(rx_timeout()));
}

bool SerialService::connectState()
{
    return connected;
}

void SerialService::handleRead()
{
QByteArray buf = serialPort->readAll();
if(!buf.contains('\n'))
  {
  rx_tail.append(buf);
  //tailChanged(rx_tail);
  }
else
  {
    QByteArray tmp;
    for(int i = 0; i < buf.length(); i++){
        tmp.append(buf[i]);
        if(buf[i] == '\n'){
            rx_tail.append(tmp);
            emit RX_Line(rx_tail.remove('\r').remove('\n'));
            rx_tail.clear();
            tmp.clear();
          }
      }
    if(tmp.length() != 0) {
        rx_tail = tmp;
      }
  }
  rx_tim->start();

}

void SerialService::handleError(QSerialPort::SerialPortError error)
{
  if (error != QSerialPort::NoError) {
        last_error = error;
        setConnectState(false);
        qDebug()<<"Port error "+ QString(error);
    emit error_msg("Port error "+ QString(error));
    }
}

bool SerialService::connect_to_port()
{
  if(serialPort == nullptr) serialPort = new QSerialPort(this);


    if((port_name.isNull())||(!QFileInfo::exists(port_name))){
        PortSelect p;
        p.setModal(true);
        if(p.exec() != QDialog::Accepted) return false;
        port_name = p.port_name();
    }


  serialPort->setPortName(port_name);

  serialPort->setBaudRate(port_baudrate);
  serialPort->setFlowControl(QSerialPort::NoFlowControl);
  last_error = QSerialPort::SerialPortError::NoError;
  bool res = serialPort->open(QIODevice::ReadWrite);
  if(res)
    {
      serialPort->clear();
      connect(serialPort, &QSerialPort::readyRead, this, &SerialService::handleRead);
      connect(serialPort, &QSerialPort::errorOccurred, this, &SerialService::handleError);
    }
  if(last_error != QSerialPort::SerialPortError::NoError) res = false;
  return res;
}

bool SerialService::disconnect_from_port()
{
  disconnect(serialPort, &QSerialPort::readyRead, this, &SerialService::handleRead);
  disconnect(serialPort, &QSerialPort::errorOccurred, this, &SerialService::handleError);
   serialPort->clear();
   serialPort->close();
   //delete serialPort;
  return true;
}

void SerialService::setConnectState(bool st)
{
    if(st != connected)
    {
    bool result = connected;
    if(st)
        result = connect_to_port();
    else
        result = !disconnect_from_port();

    if(result != connected)
        {
        connected = result;
        emit connectChanged(connected);
        }
    }

}

void SerialService::serial_send(QString s)
{
  if(connectState()){
          serialPort->write(s.toLocal8Bit());
          //serialPort.flush();
    }
}

void SerialService::rx_timeout()
{
 if(rx_tail != nullptr)
   if(rx_tail.length()>0)
     emit tailChanged(rx_tail);
}

QString SerialService::getPort_name() const
{
    return port_name;
}

void SerialService::setPort_name(const QString &value)
{
    port_name = value;
    emit port_nameChanged(port_name);
}
