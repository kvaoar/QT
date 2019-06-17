#include "gpio.h"
#include "ui_gpio.h"
#include<QDebug>
#include<QtAlgorithms>

GPIO::GPIO(QWidget *parent) :
  WSerialClient(parent),
  ui(new Ui::GPIO)
{
  ui->setupUi(this);

  ui->cPort->addItems(g_model.map_port.values());
  ui->cMode->addItems(g_model.map_dir.values());
  ui->cPull->addItems(g_model.map_pull.values());
  ui->tableView->setModel(&g_model);
}

GPIO::~GPIO()
{
  delete ui;
}

void GPIO::recieveLine(QString s){
Q_UNUSED(s);
qDebug()<<"gpio rline"<<s<< answ_len<< rx_buffer.count();
  rx_buffer.append(s);
  if(rx_buffer.count() >= answ_len){
      disconnect(serial,&SerialService::RX_Line,this,&GPIO::recieveLine);
      answ_len = 0;
      qDebug()<<"if call on_rx";
      if(on_rx_call != nullptr){
        qDebug()<<"call on_rx";
          (*this.*on_rx_call)();
        }
    }
}

void GPIO::bAdd()
{

QModelIndexList lst = ui->tableView->selectionModel()->selectedRows();
if(lst.count() <= 1){

GPIOmodel::GPIO_cfg cfg;
int from = ui->sFrom->value();
int to = ui->sTo->value();
cfg.port = g_model.map_port.key(ui->cPort->currentText());
cfg.dir = g_model.map_dir.key(ui->cMode->currentText());
cfg.pull = g_model.map_pull.key(ui->cPull->currentText());
cfg.state = ui->cLvl->isChecked();

if(to == from){
    cfg.pin = from;
    g_model.add(cfg);
    init_pin(cfg);
  } else {
for(int pin = from; pin<= to; pin++){
    cfg.pin = pin;
    g_model.add(cfg);
  }
init_pin_range(cfg, from, to);
  }
  } else {
    QList<int> editlist;
    foreach(QModelIndex ind, lst) {editlist.append(ind.row());}
    QList<GPIOmodel::GPIO_cfg> pins = g_model.at(editlist);
    QStringList keys = g_model.key_at(editlist);

    foreach(GPIOmodel::GPIO_cfg pin, pins){
        pin.dir = g_model.map_dir.key(ui->cMode->currentText());
        pin.pull = g_model.map_pull.key(ui->cPull->currentText());
        pin.state = ui->cLvl->isChecked();
        g_model.add(pin);
        init_pin(pin);
      }
  }

}

void GPIO::port_changed(QString port)
{
int maxpin = g_model.map_port_pinmax.value(g_model.map_port.key(port));
ui->sFrom->setValue(0);
ui->sTo->setValue(0);
ui->sFrom->setMaximum(maxpin);
ui->sTo->setMaximum(maxpin);

}

void GPIO::onTableClicked(const QModelIndex &index)
{


if(index.isValid()){
    qDebug()<< index.row();

    GPIOmodel::GPIO_cfg cfg = g_model.at(index.row());
    ui->cPort->setCurrentIndex(cfg.port);
    ui->cMode->setCurrentIndex(cfg.dir);
    ui->cPull->setCurrentIndex(cfg.pull);
    ui->sFrom->setValue(cfg.pin);
    ui->sTo->setValue(cfg.pin);
    ui->cLvl->setChecked(cfg.state);
  } else {
qDebug()<< "gpio invalid index";
  }
}

void GPIO::dir_changed(QString dir)
{
  if(dir == "in")
      ui->cLvl->setEnabled(false);
    else
      ui->cLvl->setEnabled(true);

};

void GPIO::on_bDel_clicked()
{
    QModelIndexList lst = ui->tableView->selectionModel()->selectedRows();
    QList<int> remlist;
    foreach(QModelIndex ind, lst) {remlist.append(ind.row());}
    QStringList pins = g_model.key_at(remlist);
    foreach(QString pin_fnm, pins) deinit_pin(pin_fnm);
    g_model.remove(pins);
ui->tableView->selectionModel()->clearSelection();
}

void GPIO::init_pin(GPIOmodel::GPIO_cfg cfg)
{
  QString pin_name = QStringLiteral("%1%2").arg(g_model.map_port.value(cfg.port)).arg(cfg.pin);
  QString mode = QStringLiteral("mode %1").arg(g_model.map_dir.value(cfg.dir));
  QString pull = QStringLiteral("pull %1").arg(g_model.map_pull.value(cfg.pull));
  QString state = (cfg.dir == GPIOmodel::PIN_Direction::IN)?"read":((cfg.state)?"on":"off");
  QString cmd = QStringLiteral("gpio %1 %2 %3 %4\n").arg(pin_name).arg(mode).arg(pull).arg(state);
  get_lines(1, &GPIO::on_rx_Read);
  emit sendStrCon(cmd);
}

void GPIO::init_pin_range(GPIOmodel::GPIO_cfg cfg, int from, int to)
{
  QString pin_name = QStringLiteral("%1%2-%3").arg(g_model.map_port.value(cfg.port)).arg(from).arg(to);
  QString mode = QStringLiteral("mode %1").arg(g_model.map_dir.value(cfg.dir));
  QString pull = QStringLiteral("pull %1").arg(g_model.map_pull.value(cfg.pull));
  QString state = (cfg.dir == GPIOmodel::PIN_Direction::IN)?"read":((cfg.state)?"on":"off");
  QString cmd = QStringLiteral("gpio %1 %2 %3 %4\n").arg(pin_name).arg(mode).arg(pull).arg(state);
  get_lines(1+to-from, &GPIO::on_rx_Read);
  emit sendStrCon(cmd);
}

void GPIO::deinit_pin(GPIOmodel::GPIO_cfg cfg)
{
  QString pin_name = QStringLiteral("%1%2").arg(g_model.map_port.value(cfg.port)).arg(cfg.pin);
 deinit_pin(pin_name);
}

void GPIO::deinit_pin(QString pin_fullname)
{
  QString cmd = QStringLiteral("gpio %1 mode in pull floating read\n").arg(pin_fullname);
  emit sendStrCon(cmd);
}

QString GPIO::make_multiport_ranges(QStringList list)
{
  QMap<GPIOmodel::PORT,QStringList*> sorted_pins;
  foreach(GPIOmodel::PORT p, g_model.map_port.keys())
    sorted_pins.insert(p, new QStringList());

  foreach(QString s, list){
      QString port = s.left(2);
      GPIOmodel::PORT p = g_model.map_port.key(port);
      sorted_pins.value(p)->append(s);
    }
  QStringList ranges;
  foreach(GPIOmodel::PORT p, g_model.map_port.keys()){
     ranges.append(make_ranges(*sorted_pins.value(p)));
    }

  QString answ;
  foreach(QString s, ranges) answ += s;
  return answ;

}

QString GPIO::make_ranges(QStringList list)
{
  int len = list.count();
  if(len==0) return "";
  if(len==1) return list.at(0);

  QString port_name = list.at(0).left(2);
  QList<int> pins;
  foreach(QString s, list) {
      int pin_num = s.right(s.length()-2).toInt();
      pins.append(pin_num);
    }

  QList<QPoint> seg;
   std::sort(pins.begin(),pins.end());
   int seg_start = 0;
   int seg_end = 0;
   for(int i = 0; i< pins.count(); i++){
       if(i+1 == pins.count()){seg.append(QPoint(seg_start,seg_end)); break;}
       if(pins.at(i) +1 != pins.at(i+1) ){
         seg.append(QPoint(seg_start,seg_end));
         seg_start = seg_end+1;
         seg_end = seg_start;
       }else
         seg_end++;
     }
   QString res;
   foreach(QPoint p, seg){
       int seg_start = pins.at(p.x());
       int seg_end = pins.at(p.y());
       if(seg_start!=seg_end)
       res += QStringLiteral("%1%2-%3 ").arg(port_name).arg(seg_start).arg(seg_end);
       else
      res += QStringLiteral("%1%2 ").arg(port_name).arg(seg_start);
     }

  return res;
}

void GPIO::get_lines(int cnt, void (GPIO::*call)())
{
  qDebug()<< "gpio getlines"<<cnt ;
  answ_len = cnt+1;
  on_rx_call = call;
  qDebug()<< connect(serial,&SerialService::RX_Line,this,&GPIO::recieveLine);
}

void GPIO::on_bRead_clicked()
{
QStringList list = g_model.key_at(GPIOmodel::PORT::SKIP_PORT, GPIOmodel::PIN_Direction::IN, GPIOmodel::PIN_Pull::SKIP_PULL);
int len = list.count();
  if(len>0){
      QString rangedpins = "";
              rangedpins.append(make_multiport_ranges(list));

      get_lines(len, &GPIO::on_rx_Read);
      QString cmd = QStringLiteral("gpio %1 read\n").arg(rangedpins);
      emit sendStrCon(cmd);
  }
}

void GPIO::on_rx_Read()
{
qDebug()<<rx_buffer;
  rx_buffer.removeAt(0);
  foreach(QString s, rx_buffer){
 QStringList answ = s.split('\t',QString::SplitBehavior::SkipEmptyParts);
 if(answ.count() == 2){
        QString pin = answ.at(0);
        int st = answ.at(1).toInt();
        g_model.lvl_changed(pin,st);
        qDebug()<<pin<<st;
      }
    }
  rx_buffer.clear();
}
