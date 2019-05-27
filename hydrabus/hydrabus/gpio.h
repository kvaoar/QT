#ifndef GPIO_H
#define GPIO_H

#include <QWidget>

namespace Ui {
  class GPIO;
}

class GPIO : public QWidget
{
  Q_OBJECT

public:
  explicit GPIO(QWidget *parent = nullptr);
  ~GPIO();

private:
  Ui::GPIO *ui;
};

#endif // GPIO_H
