#ifndef I2C_H
#define I2C_H

#include <QWidget>
#include"serialservice.h"
#include"wserialclient.h"

namespace Ui {
  class I2C;
}

class I2C : public WSerialClient
{
  Q_OBJECT

public:
  explicit I2C(QWidget *parent = nullptr);
  ~I2C();

public slots:
  void recieveLine(QString s);

private slots:
  void connect_i2c(bool st);
  void scan_cmd();
  void read_cmd();
  void write_cmd();
  void transact_cmd();
  void startstop(bool st);
  void on_bPinout_clicked();

private:
  Ui::I2C *ui;
};

#endif // I2C_H
