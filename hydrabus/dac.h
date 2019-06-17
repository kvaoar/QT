#ifndef DAC_H
#define DAC_H

#include <QWidget>
#include"serialservice.h"
#include"wserialclient.h"

namespace Ui {
  class DAC;
}


class DAC : public WSerialClient
{
  Q_OBJECT

public:
  explicit DAC(QWidget *parent = nullptr);
  ~DAC();

signals:

public slots:
  void recieveLine(QString s);

private slots:
  void connect1(bool st);
  void connect2(bool st);
  void sw_mod1(int i);
  void sw_mod2(int i);

private:
  Ui::DAC *ui;
  void cmd(int dev);

};

#endif // DAC_H
