#include "adc.h"
#include "ui_adc.h"


ADC::ADC(QWidget *parent) :
  WSerialClient(parent),
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
  chart->axes(Qt::Vertical).back()->setRange(0, 3.3);
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

void ADC::recieveLine(QString s)
{
 data.append(s);
 qDebug()<<s;

 if(data.size()>2){

     QStringList dat = s.split("\t",QString::SkipEmptyParts);
     int j = 0;
     if(dat.count() == active_chan_cnt){
     for (int i = 0;i<4;i++)
         if(chan_en.at(i)->isChecked()){
             bool ok = false;
             float res = dat.at(j++).toFloat(&ok);
             if(ok) {
                 *series.at(i)<<QPointF(series[i]->count(), qreal(res));
                // qDebug()<<series[i]->count()<<res;
               }
             else qDebug()<<"parse "<<dat;
               //setRx_en(false);
           }
       } else{
         qDebug()<<"parse "<<dat.count()<< dat;
        // setRx_en(false);
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

      foreach(QLineSeries* s, series) s->clear();
      data.clear();
      if(!ui->chkUnlim->isChecked()) chart->axes(Qt::Horizontal).back()->setRange(0,ui->sSamples->value() );
      ui->sADC_Graph->setChart(chart);
      active_chan_cnt = updSeriesVisible();
      setRx_en(true);
    } else setRx_en(false);
emit sendStrCon(cmd);

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
