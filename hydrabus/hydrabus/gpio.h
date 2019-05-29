#ifndef GPIO_H
#define GPIO_H

#include <QWidget>
#include"serialservice.h"

namespace Ui {
  class GPIO;
}

class GPIO : public QWidget
{
  Q_OBJECT

public:
  explicit GPIO(QWidget *parent = nullptr);
  ~GPIO();
  void setSerial(SerialService *value);

signals:
  void recieveEnableChanged(bool on);
  void sendStrCon(QString s);

public slots:
  void recieveLine(QString s);

private:
  Ui::GPIO *ui;
  SerialService* serial;
};

#endif // GPIO_H
