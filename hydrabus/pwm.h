#ifndef PWM_H
#define PWM_H

#include <QWidget>
#include<QTimer>
#include"serialservice.h"
#include"wserialclient.h"

namespace Ui {
  class PWM;
}




class PWM : public WSerialClient
{
  Q_OBJECT

  Q_PROPERTY(WorkState status READ getStatus WRITE setStatus)


public:
  explicit PWM(QWidget *parent = nullptr);
  ~PWM();
  typedef enum nWorkState{NO_STATE = 0, DIAL_RTCONTROL = 1, RAMP_OUT = 2 } WorkState;
  WorkState getStatus() const;
  QString help;

public slots:
  void recieveLine(QString s);

private slots:
  void guiChange(int val);
  void guiRamp(bool on);
  void guiDial(bool on);
  void guiSend();
  void tim();
  void setStatus(const WorkState &value);


private:
  Ui::PWM *ui;
  WorkState status = NO_STATE;
  QTimer* ramp_tim;
  int ramp_freq = 1;
  QString cmd_compiler(int freq, int duty);
  void RampInit();
  void RampDeInit();
  void DialInit();
  void DialDeInit();


};

#endif // PWM_H
