#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include<QByteArray>
#include<QDateTime>
#include<QLabel>

#include"timevalpoint.h"
#include "myudp.h"
#include "ftstream.h"
#include"parser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTimer* timer;
    MyUDP* udpobj;
    FTStream* datastream;
    Parser* parser;

private slots:
void on_actionExit_triggered();


private:
    Ui::MainWindow *ui;
    QLabel *label;
};

#endif // MAINWINDOW_H
