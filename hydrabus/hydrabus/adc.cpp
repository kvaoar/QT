#include "adc.h"
#include "ui_adc.h"

ADC::ADC(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ADC)
{
  chart = new QChart();
  for (int i = 0;i<4;i++) {
      QLineSeries* s = new QLineSeries();
      s->setVisible(false);
      s->setName(channel_names.at(i));
      chart->addSeries(s);
      series.append(s);
    }

  ui->setupUi(this);

  chart->createDefaultAxes();
 chart->setTheme(QChart::ChartThemeQt);
  chart->legend()->setAlignment(Qt::AlignRight);
  chart->axisY()->setRange(0, 3.3);
  ui->sADC_Graph->setChart(chart);
  ui->sADC_Graph->setVisible(false);
  chan_en.append(ui->cADC1);
  chan_en.append( ui->cTemp);
  chan_en.append( ui->cVBat);
  chan_en.append( ui->cVRef);

}

ADC::~ADC()
{
  delete ui;
}

bool ADC::getEnableRecieve()
{
  return rx_en;
}

void ADC::recieveLine(QString s)
{
 data.append(s);
 if(data.size()>3){

     QStringList dat = s.split("\t",QString::SkipEmptyParts);
     int j = 0;
     if(dat.count() == active_chan_cnt){
     for (int i = 0;i<4;i++)
         if(chan_en.at(i)->isChecked()){
             bool ok = false;
             float res = dat.at(j++).toFloat(&ok);
             if(ok) *series.at(i)<<QPointF(series[i]->count(), res);
             else setRx_en(false);
           }
       } else{
         qDebug()<<"parse "<<dat.count()<< dat;
         setRx_en(false);
       }
   }
}

void ADC::cmd()
{//adc <adc1/tempsensor/vrefint/vbat> [period (nb ms)] [samples (nb sample)/continuous]
  QString samples = QStringLiteral("samples %1").arg(ui->sSamples->value());
  if(ui->chkUnlim->isChecked()) samples = "continuous";
  QString chan = QStringLiteral("%1%2%3%4").arg(ui->cADC1->isChecked()?"adc1 ":"").arg(ui->cTemp->isChecked()?"tempsensor ":"").arg(ui->cVBat->isChecked()?"vrefint ":"").arg(ui->cVRef->isChecked()?"vbat ":"");
  QString cmd = QStringLiteral("adc %1 period %2 %3\n").arg(chan).arg(ui->sPeriod->value()).arg(samples);

  if(ui->cPlot->isChecked()){


/*        *series0 << QPointF(1, 5) << QPointF(3, 7) << QPointF(7, 6) << QPointF(9, 7)
                 << QPointF(12, 6) << QPointF(16, 7) << QPointF(18, 5);*/

      foreach(QLineSeries* s, series) s->clear();
      data.clear();
      if(!ui->chkUnlim->isChecked()) chart->axisX()->setRange(0,ui->sSamples->value() );
      ui->sADC_Graph->setChart(chart);
      active_chan_cnt = updSeriesVisible();
      setRx_en(true);
    } else setRx_en(false);
emit sendStrCon(cmd);

}

bool ADC::getRx_en() const
{
  return rx_en;
}

void ADC::setRx_en(bool value)
{
  if(rx_en != value){
  rx_en = value;
  if(rx_en) connect(serial,&SerialService::RX_Line,this,&ADC::recieveLine);
  else disconnect(serial,&SerialService::RX_Line,this,&ADC::recieveLine);
  emit recieveEnableChanged(rx_en);
  }
}


int ADC::updSeriesVisible()
{
  int res = 0;
  for (int i = 0;i<4;i++) {
      if(chan_en.at(i)->isChecked()) res++;
        series.at(i)->setVisible(chan_en.at(i)->isChecked());
    }
  return res;
}

void ADC::setSerial(SerialService *value)
{
  if(value != nullptr){
    serial = value;
    connect(this,&ADC::sendStrCon,serial,&SerialService::serial_send);
    }
}



void ADC::on_cPlot_toggled(bool checked)
{
    if(checked){
      ui->sADC_Graph->setVisible(true);
      ui->sHelp->setVisible(false);
      }else{
        ui->sADC_Graph->setVisible(false);
        ui->sHelp->setVisible(true);
      }
}
