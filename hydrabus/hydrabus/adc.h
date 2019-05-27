#ifndef ADC_H
#define ADC_H

#include <QWidget>

namespace Ui {
  class ADC;
}

class ADC : public QWidget
{
  Q_OBJECT

public:
  explicit ADC(QWidget *parent = nullptr);
  ~ADC();

private:
  Ui::ADC *ui;
};

#endif // ADC_H
