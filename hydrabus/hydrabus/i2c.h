#ifndef I2C_H
#define I2C_H

#include <QWidget>

namespace Ui {
  class I2C;
}

class I2C : public QWidget
{
  Q_OBJECT

public:
  explicit I2C(QWidget *parent = nullptr);
  ~I2C();

private:
  Ui::I2C *ui;
};

#endif // I2C_H
