#include "jtag.h"


JTAG::JTAG(QWidget *parent) :
  WSerialClient(parent),
  ui(new Ui::JTAG)
{

  ui->setupUi(this);
  state_win = new JTAGstate(this);
  ui->eCmd->addItem("");
  ui->eCmd->addItems(subscripts.keys());

  qDebug()<<"jtag on_StateWin_close: "<< connect(state_win,&JTAGstate::st_close,this, &JTAG::StatusWin_close);

}

JTAG::~JTAG()
{
  delete ui;
}

void JTAG::recieveLine(QString s)
{
  Q_UNUSED(s);
}

void JTAG::StatusWin_close(int res)
{
  qDebug()<<"jtag state closed on state "<<res;
  ui->bStateMachine->setChecked(false);
}

void JTAG::on_bConnect_toggled(bool checked)
{
  if(checked){
      emit sendStrCon(QStringLiteral("jtag pull %1 %2\n").arg(ui->cPull->currentText()).arg(ui->cMSB->currentText()));
    } else {
      emit sendStrCon("exit\n");
  }
}

void JTAG::on_bPin_clicked()
{
    emit sendStrCon("show pin\n");
}

void JTAG::on_bBypass_clicked()
{
  emit sendStrCon("bypass\n");
}

void JTAG::on_bCmd_clicked()
{

  QString cmd = ui->eCmd->currentText();
  QString scmd = junscript(cmd);

  jparse(scmd,bcmd);
  state_win->ex_cmdlist(bcmd);
  emit sendStrCon(scmd+"\n");
}


void JTAG::on_bStateMachine_toggled(bool checked)
{
  if(checked){
      state_win->show();
    }else {
      state_win->hide();
    }
}

QString JTAG::junscript(QString cmd)
{

  QString res = "";
  QStringList sl = cmd.split(" ",QString::SplitBehavior::SkipEmptyParts);
  for (int i = 0; i < sl.count(); i++) {
      QString section = sl.at(i);
      QString sub = subscripts.value(section,"");
      if(sub != "") {
          QString subop = junscript(sub);
          res += subop+" ";
        } else {
          res += section+" ";
        }
    }
  return res;
}

 void JTAG::jparse(QString cmd, QList<JTAGcmd>* cmdlist)
{
  //QList<JTAGcmd> cmdlist;
   cmdlist->clear();
  QStringList sl = cmd.split(" ",QString::SplitBehavior::SkipEmptyParts);
  for (int i = 0; i < sl.count(); i++) {
      JTAGcmd cm;
      QString seg = sl.at(i);
      if(seg.length()==1){
      if(seg == "^"){
          cm.type = JType::OP;
          cm.op = {.pin = TCK, .st = true};
          cmdlist->append(cm);
          cm.op =  {.pin = TCK, .st = false};
          cmdlist->append(cm);
        }
      JTAGOp p = map_lineop.value(sl.at(i),{NONVALID,false});
      if(p.pin != NONVALID) {
          cm.type = JType::OP;
          cm.op = p;
          cmdlist->append(cm);
        }
        }else{
          qDebug()<<seg<<" seg data";
      if((seg.left(2) == "0x")&&(seg.length() == 4)){
          bool ok = false;

          uint8_t v = seg.right(2).toInt(&ok,16);
          if(ok){
            cm.type = JType::DAT;
            cm.dat.dir = JDir::WRITE;
            cm.dat.len = 1;
            cm.dat.b_arr[0] = v;
            bool pack = false;
            // pack
            if(!cmdlist->isEmpty())
            if(cmdlist->last().type==JType::DAT)
              if(cmdlist->last().dat.dir == JDir::WRITE)
                if(cmdlist->last().dat.len<255){
                    cmdlist->last().dat.b_arr[cmdlist->last().dat.len] = v;
                    cmdlist->last().dat.len ++;
                    pack = true;
                    qDebug()<<"pack";
                  }
            //end pack
            if(!pack) {
                cmdlist->append(cm);
                    qDebug()<<"append"<<cm.dat.b_arr[0];
              }
            }
        }

      if(seg.left(2) == "r:"){
          bool ok = false;
          uint8_t len = seg.right(seg.length()-2).toInt(&ok,10);
          if(ok){
              cm.type = JType::DAT;
              cm.dat.dir = JDir::READ;
              cm.dat.len = len;
              memset(cm.dat.b_arr,0,256);
            bool pack = false;
              // pack
              if(cmdlist->last().type==JType::DAT)
                if(cmdlist->last().dat.dir == JDir::READ)
                  if(cmdlist->last().dat.len<255){
                      cmdlist->last().dat.len ++;
                      pack = true;
                    }
              //end pack
              if(!pack) {
                  cmdlist->append(cm);
                }
            }

        }
    }
    }

}



