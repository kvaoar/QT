#ifndef JTAG_H
#define JTAG_H

#include <QWidget>
#include<QDebug>
#include"serialservice.h"
#include "ui_jtag.h"
#include"jtagstate.h"
#include"wserialclient.h"
#include"jtag_cmd.h"

namespace Ui {
  class JTAG;
}

class JTAG : public WSerialClient
{
  Q_OBJECT

public:
  explicit JTAG(QWidget *parent = nullptr);
  ~JTAG();


public slots:
  void recieveLine(QString s);
  void StatusWin_close(int res);

private slots:
  void on_bConnect_toggled(bool checked);
  void on_bPin_clicked();
  void on_bBypass_clicked();

  void on_bCmd_clicked();
  void on_bStateMachine_toggled(bool checked);

private:
  Ui::JTAG *ui;
  JTAGstate* state_win;
  QList<JTAGcmd>* bcmd = new QList<JTAGcmd>();
  QString junscript(QString);
  void jparse(QString,QList<JTAGcmd>*);
  const QMap<QString,JTAGOp> map_lineop={
    {"/", {TCK,true}},
    {"\\", {TCK,false}},
    {"[", {TMS,true}},
    {"]", {TMS,false}},
    {"-", {TDI,true}},
    {"_", {TDI,false}}
  };
  QMap<QString,QString> subscripts = {
    {"HALT","[ ^ [ ^ [ ^ [ ^ [ ^ ] ^"},
    {"DATA","[ ^ ] ^ ] ^"},
    {"INDEX","[ ^ [ ^ ] ^ ] ^"}
  };

};

#endif // JTAG_H
