#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QFileDialog>

#include<QTimer>
#include"Cursors/metacursor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    udpobj = new MyUDP(this);
    series_stor = new SeriesStorage(this);
    cur_stor = new CursorStorage(this);
    mcur = new MouseCur(this);
    scaler = new Scaler(this);

    scaler->setStorage(series_stor); //refresh remove lasttime
    mcur->setScaler(scaler);
    cur_stor->setScaler(scaler);

    ui->openGLWidget->setScaler(scaler);

    ui->combostackWidget->addItems({"Settings","Cursors"});

    connect((ui->actionSave), SIGNAL(triggered()),series_stor,SLOT(Save() ));
    connect((ui->actionLoad), SIGNAL(triggered()),series_stor,SLOT(Load() ));
    connect((ui->actionReset),SIGNAL(triggered()),series_stor,SLOT(Reset()));
    connect((ui->actionConnect),SIGNAL(toggled(bool)), udpobj,SLOT(udp_connect(bool)));
    connect((ui->actionScan),SIGNAL(toggled(bool)), udpobj,SLOT(udp_rescanst(bool)));
    connect(udpobj,SIGNAL(is_connected(bool)), (ui->actionConnect),SLOT(setChecked(bool)));
    connect(udpobj, SIGNAL(dataout(QString,SensorState* ,TimeValPoint)), series_stor, SLOT(Add(QString,SensorState*,TimeValPoint)));
    connect(series_stor, &SeriesStorage::last_time, cur_stor,&CursorStorage::ser_onLastTime);

    connect(series_stor,&SeriesStorage::color_changed,cur_stor,&CursorStorage::ser_onColorChanged);

    connect(mcur, &MouseCur::newcursor, cur_stor, &CursorStorage::add);
    connect(mcur, &MouseCur::delcursor, cur_stor, &CursorStorage::del);

    connect((ui->actionSetAbs),SIGNAL(toggled(bool)),mcur,SLOT(abs_cursor(bool)));
    connect(ui->openGLWidget,SIGNAL(abs_cursor(bool)),mcur,SLOT(abs_cursor(bool)));

    connect((ui->actionDots), SIGNAL(toggled(bool)),ui->openGLWidget,SLOT(set_dots(bool)));


    connect(series_stor, &SeriesStorage::statusChanged, this, &MainWindow::status);
    connect(scaler, &Scaler::statusChanged, this, &MainWindow::status);


    //connect(cur_stor,SIGNAL(refresh_cursors(QVector<Cursor>*)),ui->openGLWidget,  SLOT(refresh_cursors(QVector<Cursor>*)));
    //connect(cur_stor,SIGNAL(refresh_mcursors(QVector<MetaCursor>*)),ui->openGLWidget,  SLOT(refresh_mcursors(QVector<MetaCursor>*)));


    connect(series_stor, SIGNAL(lastval(QString,QString,QString)), this, SLOT(lastval_rx(QString,QString,QString)));
    connect((ui->actionLinLog),&QAction::toggled,scaler,&Scaler::set_lin);

udpobj->model()->SetTableControls(this->ui->tableHost);
series_stor->set_series_table(this->ui->tableSeries);
cur_stor->set_tables(this->ui->tableCursor,this->ui->tableCGrp);
scaler->setXComboBox(ui->comboXstep);
ui->openGLWidget->setView(ui->listGlColor);
udpobj->udp_connect(true);

}

MainWindow::~MainWindow()
{
    delete udpobj;
    delete series_stor;
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::lastval_rx(QString host, QString val,QString col)
{
    if(labels.contains(host)){
        labels.value(host)->setText(val+" ");
    } else {
            QLabel* l = new QLabel(this);
            l->setFont(QFont("Arial", 14, QFont::Bold));
            l->setStyleSheet(QString("color:%1;").arg(QColor(col).darker(100).name()));
            labels.insert(host,l);
            ui->ValuesToolBar->addWidget(labels.value(host));
    }
}

void MainWindow::on_actionDebugIn_toggled(bool arg1)
{
    udpobj->udp_debug(arg1);
}

void MainWindow::status(QString s)
{
    QString name = sender()->objectName();
    _status[name] = s;

    QString res = "";
    foreach(QString tag, _status.keys()){
        res += tag + " : " + _status[tag];
    }
    ui->statusBar->showMessage(res,3000);
    _status.clear();
}


