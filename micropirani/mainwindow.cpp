#include "mainwindow.h"
#include "ui_mainwindow.h"

    QList<QPointF> points;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QPalette pal = window()->palette();
       pal.setColor(QPalette::Window, QRgb(0x252525));
       pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
       window()->setPalette(pal);
    ui->setupUi(this);

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAddInfo_triggered()
{
int s = points.size();
        for(int i = s; i < s+1000; i++){
            float x = i*2.0*3.1415/1000.0;
            float y = 100.0*(2+sin(x));
            points.append(QPointF(x,y));}

        ui->openGLWidget->setPoints(points);
}
