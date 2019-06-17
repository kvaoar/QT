#ifndef GPIO_H
#define GPIO_H

#include <QWidget>
#include"serialservice.h"
#include"gpiomodel.h"
#include<QModelIndex>
#include"wserialclient.h"

namespace Ui {
  class GPIO;
}



class GPIO : public WSerialClient
{
  Q_OBJECT


public:
  explicit GPIO(QWidget *parent = nullptr);
  ~GPIO();

public slots:
  void recieveLine(QString s);
  void bAdd();
  void port_changed(QString port);

private slots:
  void onTableClicked(const QModelIndex &index);
  void dir_changed(QString dir);
  void on_bDel_clicked();
  void on_bRead_clicked();

private:

  Ui::GPIO *ui;
  int answ_len = 0;
  void(GPIO::*on_rx_call)() = nullptr;
  QStringList rx_buffer;
  void on_rx_Read();
  GPIOmodel g_model;
  void init_pin(GPIOmodel::GPIO_cfg cfg);
  void init_pin_range(GPIOmodel::GPIO_cfg cfg, int from, int to);
  void deinit_pin(GPIOmodel::GPIO_cfg cfg);
  void deinit_pin(QString pin_fullname);
  QString make_multiport_ranges(QStringList list);
  QString make_ranges(QStringList list);
  void get_lines(int cnt, void(GPIO::*call)());

};

#endif // GPIO_H
