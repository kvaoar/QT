#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include<QByteArray>
#include<QDateTime>
#include<QLabel>
#include "Sensors/myudp.h"
#include "Cursors/cursorstorage.h"
#include"Cursors/mousecur.h"
#include "Series/seriesstorage.h"
#include"Series/scaler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MyUDP* udpobj;
    SeriesStorage* series_stor;
    Scaler* scaler;
    MouseCur* mcur;
    CursorStorage* cur_stor;


private slots:
void on_actionExit_triggered();
void lastval_rx(QString host, QString val, QString col);
void on_actionDebugIn_toggled(bool arg1);
void status(QString s);

private:
    Ui::MainWindow *ui;
    QMap<QString,QLabel*> labels;
    QMap<QString, QString> _status;
};

#endif // MAINWINDOW_H
