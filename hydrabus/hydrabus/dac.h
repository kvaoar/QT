#ifndef DAC_H
#define DAC_H

#include <QWidget>

namespace Ui {
  class DAC;
}

class DAC : public QWidget
{
  Q_OBJECT

public:
  explicit DAC(QWidget *parent = nullptr);
  ~DAC();

private:
  Ui::DAC *ui;
};

#endif // DAC_H
