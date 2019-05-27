#ifndef UART_H
#define UART_H

#include <QWidget>

namespace Ui {
  class UART;
}

class UART : public QWidget
{
  Q_OBJECT

public:
  explicit UART(QWidget *parent = nullptr);
  ~UART();

private:
  Ui::UART *ui;
};

#endif // UART_H
