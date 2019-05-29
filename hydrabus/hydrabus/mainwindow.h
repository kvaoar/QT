#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QtCore>
#include <QMainWindow>
#include<QSerialPort>
#include <QCloseEvent>
#include <QTextStream>
#include <QTimer>
#include <QDebug>
#include <QSerialPortInfo>
#include<QMessageBox>
#include<QTextEdit>
#include<QTextStream>
#include <QFileInfo>
#include <QtCharts>

#include "console.h"
#include "serialservice.h"
#include"pwm.h"
#include"adc.h"
#include"dac.h"
#include"gpio.h"
#include"spi.h"
#include"i2c.h"
#include"uart.h"
#include"jtag.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void setTabIndex(int i);
    void gui_connectionproc(bool state);
    void closeEvent (QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    QString serial_port_name;
    SerialService *serial;
};

#endif // MAINWINDOW_H
