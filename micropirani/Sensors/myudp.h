#ifndef MYUDP_H
#define MYUDP_H

#include <QObject>
#include <QUdpSocket>
#include<QDateTime>
#include <QHostInfo>
#include <QMap>
#include<QTimer>
#include<Sensors/sensorstate.h>
#include<QtMath>
#include<QRandomGenerator>
#include "Units/torr.h"
#include "Series/series.h"
#include"Tables/nettable.h"

class MyUDP : public QObject
{
    Q_OBJECT
public:
    explicit MyUDP(QObject *parent = nullptr);
    void add(QString host, QString ask, SParser* p=nullptr);
    int cnt();
    NetTable* model(){return n_tab;}
signals:
    void is_connected(bool state);
    void dataout(QString host, SensorState* sensor, TimeValPoint p);
public slots:
    void udp_connect(bool state);
    void udp_debug(bool state);
    void udp_ask();
    void udp_read();
    void udp_rescan();
    void udp_rescanst(bool on);
private:
    NetTable* n_tab;
    QString try_ip(QString host);
    QMap<QString,QString> ip_to_host;
    QMap<QString,SensorState> host_to_sensor;
    QRandomGenerator* ran;
    QUdpSocket *socket;
    bool _is_connected = false;
    bool _is_debug = false;
    QTimer* rescan_timer;
    QTimer* ask_timer;
};

Q_DECLARE_METATYPE(MyUDP*)

#endif
