#ifndef IINTERFACE_H
#define IINTERFACE_H

#include <QWidget>

namespace Ui {
  class IInterface;
}

class IInterface : public QWidget
{
  Q_OBJECT

public:
  explicit IInterface(QWidget *parent = nullptr);
  ~IInterface();

private:
  Ui::IInterface *ui;
};

#endif // IINTERFACE_H
