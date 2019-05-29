#ifndef I2C_H
#define I2C_H

#include <QWidget>
#include"serialservice.h"
namespace Ui {
  class I2C;
}

class I2C : public QWidget
{
  Q_OBJECT

public:
  explicit I2C(QWidget *parent = nullptr);
  ~I2C();
void setSerial(SerialService *value);

signals:
  void recieveEnableChanged(bool on);
  void sendStrCon(QString s);

public slots:
  void recieveLine(QString s);

private slots:
  void connect_i2c(bool st);
  void scan_cmd();
  void read_cmd();
  void write_cmd();
  void transact_cmd();
  void startstop(bool st);

private:
  Ui::I2C *ui;
  SerialService* serial;
};

#endif // I2C_H
