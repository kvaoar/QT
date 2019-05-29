#ifndef SPI_H
#define SPI_H

#include <QWidget>
#include"serialservice.h"
namespace Ui {
  class SPI;
}

class SPI : public QWidget
{
  Q_OBJECT

public:
  explicit SPI(QWidget *parent = nullptr);
  ~SPI();
    bool getEnableRecieve();
    void setSerial(SerialService *value);

signals:
  void recieveEnableChanged(bool on);
  void sendStrCon(QString s);

public slots:
  void recieveLine(QString s);

private slots:
  void setSPI(bool st);
  void setDevice(int index);
  void cmdRead();
  void cmdWrite();
  void cmdPinout();
  void cmdNSS(bool st);
  void cmdDump();


private:
  void spi_connect();
  void spi_disconnect();
  QString cmd();
  Ui::SPI *ui;
  bool spi_on = false;
  bool rx_en = false;
  const QStringList dev1 = {        "320000",
                              "650000",
                              "1310000",
                              "2620000",
                              "5250000",
                              "10500000",
                              "21000000",
                              "42000000"};
  const QStringList dev2 = {        "160000",
                              "320000",
                              "650000",
                              "1310000",
                              "2620000",
                              "5250000",
                              "10500000",
                              "21000000"

                      };
  SerialService* serial;

};

#endif // SPI_H
