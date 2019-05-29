#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include"serialservice.h"

namespace Ui {
class Console;
}

class Console : public QWidget
{
    Q_OBJECT

   Q_PROPERTY(bool connect_status READ getConnect_status WRITE setConnect_status NOTIFY connect_statusChanged)

public:
    explicit Console(QWidget *parent = nullptr);
    ~Console();

  bool getConnect_status() const;
  void setSerial(SerialService *value);

public slots:
  void setConnect_status(bool value);
  void appendLine(QString s);
  void setStatus(QString s);
  void setTail(QString s);

signals:
  void connect_statusChanged(bool st);
  void send_cmd(QString s);

private slots:
  void on_bManual_clicked(bool checked);

private:
  Ui::Console *ui;
  bool connect_status;
  QString tail;
  SerialService* serial;
};

#endif // CONSOLE_H
