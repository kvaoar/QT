#ifndef PWM_H
#define PWM_H

#include <QWidget>
#include<QTimer>
#include"serialservice.h"

namespace Ui {
  class PWM;
}

typedef enum {
  NO_STATE = 0,
  DIAL_RTCONTROL = 1,
  RAMP_OUT = 2
} WorkState;

class PWM : public QWidget
{
  Q_OBJECT

  Q_PROPERTY(WorkState status READ getStatus WRITE setStatus)

private slots:
  void guiChange(int val);
  void guiRamp(bool on);
  void guiDial(bool on);
  void guiSend();
  void tim();
  void setStatus(const WorkState &value);

public:
  explicit PWM(QWidget *parent = nullptr);
  ~PWM();
  WorkState getStatus() const;
  QString help;
  void setSerial(SerialService *value);


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
  SerialService* serial;



  // ICon interface
signals:
  void recieveEnableChanged(bool on);
  void sendStrCon(QString s);

public slots:
  void recieveLine(QString s);

public:
  bool getEnableRecieve();
};

#endif // PWM_H
