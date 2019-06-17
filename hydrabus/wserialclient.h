#ifndef SERIALCALLBACKWIDGET_H
#define SERIALCALLBACKWIDGET_H

#include <QObject>
#include <QWidget>
#include"serialservice.h"

class WSerialClient : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(bool rx_en READ getRx_en WRITE setRx_en)
public:
  explicit WSerialClient(QWidget *parent = nullptr);
  void setSerial(SerialService *value);
  bool getEnableRecieve();
  bool getRx_en() const;
  void setRx_en(bool value);

signals:
  void sendStrCon(QString s);
  void recieveEnableChanged(bool en);

public slots:
  virtual void recieveLine(QString s) = 0;

public:


protected:
  bool rx_en = false;
SerialService* serial;
private:

};

#endif // SERIALCALLBACKWIDGET_H
