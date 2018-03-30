#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QFileDialog>
//#include <QQmlContext>
#include <QtQuick/QQuickView>

#include<QQmlContext>
#include<QQmlEngine>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    timer = new QTimer(this);
    parser = new Parser(this);
    datastream = new FTStream(this);
    udpobj = new MyUDP(this);

    connect(timer, SIGNAL(timeout()), parser, SLOT(on_data_ask()));
    connect(timer, SIGNAL(timeout()), udpobj, SLOT(udp_ask()));

    connect(udpobj,SIGNAL(is_connected(bool)), (ui->actionConnect),SLOT(setChecked(bool)));
    connect((ui->actionConnect),SIGNAL(toggled(bool)), udpobj,SLOT(udp_connect(bool)));

    connect((ui->actionSave), SIGNAL(triggered()),datastream,SLOT(Save() ));
    connect((ui->actionLoad), SIGNAL(triggered()),datastream,SLOT(Load() ));
    connect((ui->actionReset),SIGNAL(triggered()),datastream,SLOT(Reset()));

    connect(udpobj, SIGNAL(udp_recieved(QByteArray,QDateTime)), parser, SLOT(on_data_rec(QByteArray,QDateTime)));
    connect(parser, SIGNAL(dataout(TimeValPoint)), datastream, SLOT(add(TimeValPoint)));

    connect(datastream,SIGNAL(refresh(QList<TimeValPoint>*)),parser,            SLOT( on_data_upd(QList<TimeValPoint>*)));
    connect(datastream,SIGNAL(refresh(QList<TimeValPoint>*)),ui->openGLWidget,  SLOT(refresh_data(QList<TimeValPoint>*)));

    connect((ui->actionLinLog),SIGNAL(toggled(bool)),(ui->openGLWidget),SLOT(set_ax_y_lin(bool)));


  //  QQuickView qview;
    qmlRegisterType<CursorStorage>("my.cursorstorage", 1, 0, "CursorStorage");

    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    ui->quickWidget->engine()->rootContext()->setContextProperty("dataModel",QVariant::fromValue(ui->openGLWidget->get_curlist()));
    ui->quickWidget->setSource( QUrl("qrc:/CurList.qml"));
    //QQmlContext* ctxt = ui->quickWidget->rootContext();


    timer->start(500);
    label = new QLabel(this);
    label->setFont(QFont("Arial", 16, QFont::Bold));
    ui->ValuesToolBar->addWidget(label);

    connect(parser, SIGNAL(lastval(QString)), label, SLOT(setText(QString)));
    udpobj->udp_connect(true);
}

MainWindow::~MainWindow()
{

    delete timer;
    delete udpobj;
    delete datastream;
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}




