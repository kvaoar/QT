#ifndef ADC_H
#define ADC_H

#include <QWidget>
#include<QtCharts>
#include"serialservice.h"

namespace Ui {
  class ADC;
}

class ADC : public QWidget
{
  Q_OBJECT
Q_PROPERTY(bool rx_en READ getRx_en WRITE setRx_en)

public:
  explicit ADC(QWidget *parent = nullptr);
  ~ADC();
   bool getEnableRecieve();

   bool getRx_en() const;
   void setRx_en(bool value);

   void setSerial(SerialService *value);

public slots:
   void recieveLine(QString s);
   void cmd();

signals:
  void recieveEnableChanged(bool on);
  void sendStrCon(QString s);

private slots:


  void on_cPlot_toggled(bool checked);

private:
  Ui::ADC *ui;
  bool rx_en = false;
  QChart* chart;
  const QStringList channel_names = {"adc","tempsensor","vrefint","vbat"};
  QList<QLineSeries*> series;
  QList<QCheckBox*> chan_en;
  int active_chan_cnt = 1;
  QStringList data;
  int updSeriesVisible();

  SerialService* serial;
};

#endif // ADC_H
