#ifndef DAC_H
#define DAC_H

#include <QWidget>
#include"serialservice.h"

namespace Ui {
  class DAC;
}


class DAC : public QWidget
{
  Q_OBJECT

public:
  explicit DAC(QWidget *parent = nullptr);
  ~DAC();
void setSerial(SerialService *value);


signals:
  void recieveEnableChanged(bool on);
  void sendStrCon(QString s);

public slots:
  void recieveLine(QString s);

private slots:
  void connect1(bool st);
  void connect2(bool st);
  void sw_mod1(int i);
  void sw_mod2(int i);

private:
  Ui::DAC *ui;
  SerialService* serial;

  void cmd(int dev);

};

#endif // DAC_H
