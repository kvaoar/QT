#ifndef UART_H
#define UART_H

#include <QWidget>
#include"serialservice.h"
namespace Ui {
  class UART;
}

class UART : public QWidget
{
  Q_OBJECT

public:
  explicit UART(QWidget *parent = nullptr);
  ~UART();
void setSerial(SerialService *value);

signals:
  void recieveEnableChanged(bool on);
  void sendStrCon(QString s);

public slots:
  void recieveLine(QString s);

private:
  Ui::UART *ui;
  SerialService* serial;
};

#endif // UART_H
