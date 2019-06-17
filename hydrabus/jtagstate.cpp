#include "jtagstate.h"
#include "ui_jtagstate.h"

#include<QDebug>

JTAGstate::JTAGstate(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::JTAGstate)
{

  ui->setupUi(this);
  //ui->gView->setDisabled(true);
  machine = new jtag_sm();

  connect(machine,&jtag_sm::log,this,&JTAGstate::stlog);

  machine->start();

  //svgwidget = new SvgJtag(QStringLiteral(":/res/JTAG_TAP_Controller_State_Diagram.svg"));


  //svgrend = new QSvgRenderer();
  scene.setParent(ui->gView);
  ui->gView->setScene(&scene);

  //ui->gView->setRenderHint(QPainter::RenderHint::Antialiasing);
  //qgsvgit->setSharedRenderer(svgrend);



led.setRect(QRect(0,0,20,20));
led.setBrush(Qt::red);
dr_val.setPos(QPoint(10,500));
ir_val.setPos(QPoint(10,1000));
dr_val.setFont(QFont("Arial", 20, QFont::Normal));
ir_val.setFont(QFont("Arial", 20, QFont::Normal));
dr_val.setPlainText("DR: 0x00");
ir_val.setPlainText("IR: 0x00");
scene.addItem(&qgsvgit);
scene.addItem(&led);
scene.addItem(&dr_val);
scene.addItem(&ir_val);

connect(machine,&QScxmlStateMachine::reachedStableState,this,&JTAGstate::upd_state);
//  scene.addItem(led);


}

JTAGstate::~JTAGstate()
{
  delete ui;
}

void JTAGstate::tck(bool lvl)
{
  //qDebug()<<"tck"<<lvl;
  if(lvl != level.tck){
  if(lvl){
      qDebug()<<"tms"<<lvl;
      if(level.tms)
        machine->submitEvent("HI");
      else
        machine->submitEvent("LOW");
    } else {
/**/
    }
    level.tck = lvl;
  }
}

void JTAGstate::tms(bool lvl)
{

  if(level.tms != lvl) level.tms = lvl;
}

void JTAGstate::tdi(bool lvl)
{

}

void JTAGstate::tdo(bool lvl)
{

}


void JTAGstate::ex_op(JTAGOp command)
{

  switch (command.pin) {
    case TCK:
      tck(command.st);
      break;
    case TMS:
      tms(command.st);
      break;
    case TDI:
      tdi(command.st);
      break;
    default:
      break;
    }

}

void JTAGstate::ex_dat(JTAGDat command)
{

qDebug()<<"ex_dat cmd mach state"<<machine->activeStateNames();
  QGraphicsTextItem* label = nullptr;
  QString dir;
  if(command.dir == JDir::WRITE){ dir = "wr";}
  else
    if(command.dir == JDir::READ){ dir = "rd";}
    else {return;}

      QString jreg;



      if(machine->isActive(st_map.value(SHIFT_DR))){
          jreg = "DR";
          label = &dr_val;
        } else
      if(machine->isActive(st_map.value(SHIFT_IR))){
          jreg = "IR";
          label = &ir_val;
        } else {

          return;
        }

      if(label != nullptr) {
          QString data;
          for (int i = 0; i < command.len; i++) {
              uint8_t d = command.b_arr[i];
              data += QString("0x%1 ").arg(d,0,16);
            }
          label->setPlainText(dir+jreg+data);
          qDebug()<<dir+jreg+data;
        }

}

void JTAGstate::ex_cmdlist(QList<JTAGcmd>* commands)
{
  for (int i = 0; i < commands->length();i++) {
      qDebug()<<"cmd type "<<commands->at(i).type;
      qDebug()<<"mach state"<<machine->activeStateNames();
      if(commands->at(i).type == JType::OP)ex_op(commands->at(i).op);
      if(commands->at(i).type == JType::DAT)ex_dat(commands->at(i).dat);
    }
}


void JTAGstate::on_bHi_clicked()
{
    machine->submitEvent("HI");

}

void JTAGstate::on_bLow_clicked()
{
    machine->submitEvent("LOW");

}


void JTAGstate::upd_state()
{
  qDebug()<<"JTAGstate::upd_state()"<<machine->activeStateNames();
JStates st = st_map.key(machine->activeStateNames().at(0));
QPoint p = st_led.value(st);
led.setX(p.x());
led.setY(p.y());
//qDebug()<<"state up "<<led.rect();
//qDebug()<<machine.activeStateNames();
}

void JTAGstate::stlog(const QString &label, const QString &msg)
{
  qDebug()<<label<< msg;
}

void JTAGstate::showEvent(QShowEvent *eve)
{
   if(eve->type() == QShowEvent::Type::Show){
    if(!jshow){

       qDebug()<<"open"<<eve;
        ui->gView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
              jshow = true;
      }
     }
   else QDialog::showEvent(eve);
}


void JTAGstate::closeEvent(QCloseEvent * event)
{
  QStringList s = machine->activeStateNames();
  if(event->type() == QCloseEvent::Type::Close){qDebug()<<"close"<<event; emit st_close(st_map.key(s.at(0)));}
  else QDialog::closeEvent(event);
}

void JTAGstate::resizeEvent(QResizeEvent *event)
{

  if(event->type() == QResizeEvent::Type::Resize){
      qDebug()<<"resize "<<event<<" framesize"<<ui->gView->frameSize() ;

     ui->gView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);

    }
  else QDialog::resizeEvent(event);
}


