#ifndef ADC_H
#define ADC_H

#include <QWidget>
#include<QtCharts>
#include"serialservice.h"
#include"wserialclient.h"

namespace Ui {
  class ADC;
}

class ADC : public WSerialClient
{
  Q_OBJECT

public:
  explicit ADC(QWidget *parent = nullptr);
  ~ADC();

public slots:
   void recieveLine (QString s);
   void cmd();

private slots:
  void on_cPlot_toggled(bool checked);

private:
  Ui::ADC *ui;
  QChart* chart;
  const QStringList channel_names = {"adc","tempsensor","vrefint","vbat"};
  QList<QLineSeries*> series;
  QList<QCheckBox*> chan_en;
  int active_chan_cnt = 1;
  QStringList data;
  int updSeriesVisible();

};

#endif // ADC_H
