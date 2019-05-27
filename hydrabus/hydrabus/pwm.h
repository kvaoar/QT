#ifndef PWM_H
#define PWM_H

#include <QWidget>

namespace Ui {
  class PWM;
}

class PWM : public QWidget
{
  Q_OBJECT

public:
  explicit PWM(QWidget *parent = nullptr);
  ~PWM();

private:
  Ui::PWM *ui;
};

#endif // PWM_H
