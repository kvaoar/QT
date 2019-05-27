#ifndef JTAG_H
#define JTAG_H

#include <QWidget>

namespace Ui {
  class JTAG;
}

class JTAG : public QWidget
{
  Q_OBJECT

public:
  explicit JTAG(QWidget *parent = nullptr);
  ~JTAG();

private:
  Ui::JTAG *ui;
};

#endif // JTAG_H
