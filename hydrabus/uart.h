#ifndef UART_H
#define UART_H

#include <QWidget>
#include"serialservice.h"
#include"wserialclient.h"
namespace Ui {
  class UART;
}

class UART : public WSerialClient
{
  Q_OBJECT

public:
  explicit UART(QWidget *parent = nullptr);
  ~UART();

public slots:
  void recieveLine(QString s);

private slots:
  void on_bConnect_toggled(bool checked);

  void on_bPinout_clicked();

  void on_bWrite_clicked();

  void on_bRead_clicked();

private:
  Ui::UART *ui;
};

#endif // UART_H
