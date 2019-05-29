#ifndef JTAG_H
#define JTAG_H

#include <QWidget>
#include"serialservice.h"

namespace Ui {
  class JTAG;
}

class JTAG : public QWidget
{
  Q_OBJECT

public:
  explicit JTAG(QWidget *parent = nullptr);
  ~JTAG();
void setSerial(SerialService *value);

signals:
  void recieveEnableChanged(bool on);
  void sendStrCon(QString s);

public slots:
  void recieveLine(QString s);

private:
  Ui::JTAG *ui;
  SerialService* serial;
};

#endif // JTAG_H
