#ifndef SPI_H
#define SPI_H

#include <QWidget>

namespace Ui {
  class SPI;
}

class SPI : public QWidget
{
  Q_OBJECT

public:
  explicit SPI(QWidget *parent = nullptr);
  ~SPI();

private:
  Ui::SPI *ui;
};

#endif // SPI_H
