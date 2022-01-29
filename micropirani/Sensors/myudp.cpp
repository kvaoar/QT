#include "myudp.h"
#include"Parsers/mks925c.h"
#include"Parsers/imr265.h"

MyUDP::MyUDP(QObject *parent)
   : QObject(parent)
{
    _is_connected = false;
    n_tab = new NetTable(this);
    n_tab->SetHostMap(&host_to_sensor);



    add("espsensor1.lan","@253PR1?;FF\n",new MKS925C);
    add("espsensor2.lan","A\n",new IMR265);

    socket = new QUdpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(udp_read()));
    ask_timer = new QTimer(this);
    ask_timer->setInterval(100);
    rescan_timer = new QTimer(this);
    rescan_timer->setInterval(10000);
    connect(ask_timer, SIGNAL(timeout()), this, SLOT(udp_ask()));
   connect(rescan_timer, SIGNAL(timeout()), this, SLOT(udp_rescan()));



}

QString MyUDP::try_ip(QString host)
{
    QHostInfo info = QHostInfo::fromName(host);
    if (!info.addresses().isEmpty()) {
    QHostAddress ip = info.addresses().first();
    return ip.toString();
    }
    qDebug()<<host<<" not found";
    return "";
}

void MyUDP::add(QString host, QString ask, SParser *p)
{
    SensorState s;
    s.host = host;
    s.ask = ask;

    s.p = p;
    s.ip = try_ip(host);
    s.series_ranges = p->ranges();


    if(s.ip.compare("")==0)
        s.is_connected = false;
    else
        s.is_connected = true;


    host_to_sensor.insert(host,s);
    n_tab->ext_upd(host,NetTable::SC_ALL);

    if(s.is_connected){
        ip_to_host.insert(s.ip,s.host);
    }

}

int MyUDP::cnt()
{
    return  host_to_sensor.size();
}

void MyUDP::udp_ask() //ask each record in ip-keyed map
{

if(_is_connected){ // if communication enabled

    if(_is_debug){

        foreach(QString ip, ip_to_host.keys()){ // for every availible ip adress

            QString host = ip_to_host.value(ip);    //get host name
            QString ask = host_to_sensor.value(host).ask; // get ask format from host table
            QByteArray Data;
            Data.append(ask);

            host_to_sensor[host].last_time = QDateTime::currentDateTime().toMSecsSinceEpoch(); //get timestamp
            foreach(QString ser, host_to_sensor[host].series_ranges.keys()){
             //qDebug()<<ip<<"dbg ASK:"<< Data;
                QPair<qreal,qreal> r = host_to_sensor[host].series_ranges[ser];
            ran->seed(QDateTime::currentDateTime().toMSecsSinceEpoch());
             host_to_sensor[host].series_val[ser] += (ran->bounded(5.0*r.second)-((5.0*r.second)/2.0) );
             if(host_to_sensor[host].series_val[ser]> r.first) host_to_sensor[host].series_val[ser] = r.first;
             if(host_to_sensor[host].series_val[ser]< r.second) host_to_sensor[host].series_val[ser] = r.second;
             //qDebug()<<"DBG GENERATOR"<<host<<ser<<host_to_sensor[host].series_val[ser]<<host_to_sensor[host].last_time;
            }


             emit dataout(host, &host_to_sensor.find(host).value(),TimeValPoint{host_to_sensor[host].last_time,host_to_sensor[host].series_val }); //emit message to storage
             }

        return;
    } else {

foreach(QString ip, ip_to_host.keys()){ // for every availible ip adress
    QString host = ip_to_host.value(ip);    //get host name
    QString ask = host_to_sensor.value(host).ask; // get ask format from host table
    QByteArray Data;
    Data.append(ask);
    socket->writeDatagram(Data, QHostAddress(ip), 8888); // write UDP datagramm
     //qDebug()<<ip<<"ASK:"<< Data;
     }
 }
}
}

//udp recieve callback
void MyUDP::udp_read()
{
    QByteArray answer;
    QDateTime time = QDateTime::currentDateTime(); //get timestamp
    qint64 size = socket->pendingDatagramSize(); //get datasize from udp
    answer.resize(static_cast<int>(size));  //set bytearray to this size
    QHostAddress ip;
    socket->readDatagram(answer.data(), answer.size(), &ip); //read datagramm from socket
    qDebug()<< answer;
    QString s_ip = QHostAddress(ip.toIPv4Address()).toString(); //convert ip from multiformat trash
    if(!ip_to_host.contains(s_ip)) { // break if ip not in list
        qDebug()<< answer <<" recieved from UFO" <<s_ip;
        return;
    }
    QString host = ip_to_host.value(s_ip);  // get hostname from ip
    SensorState* ss = &host_to_sensor.find(host).value();
    ss->series_val = host_to_sensor.value(host).p->parser(answer); // use converter associated with hostname
    emit dataout(host, ss, TimeValPoint{time.toMSecsSinceEpoch(),ss->series_val}); //emit message to storage

}



void MyUDP::udp_rescan()
{

    if(_is_debug){
        int i = 0;

        foreach(QString host, host_to_sensor.keys()){
             SensorState* st = &host_to_sensor.find(host).value();

             if(st->is_connected == false){
             st->is_connected = true;
             st->ip = QString("1.1.1.%1").arg(i++);
             ip_to_host[st->ip]=host;
             qDebug()<<"dbg_scan "<<host_to_sensor.value(host).host<<host_to_sensor.value(host).ip<<host_to_sensor.value(host).is_connected;

             n_tab->ext_upd(host,NetTable::SC_IP);
             n_tab->ext_upd(host,NetTable::SC_CONN);
             }


        }



        return;
    } else {


    foreach(QString host, host_to_sensor.keys()){
        QString ip = try_ip(host);
        SensorState* st = &host_to_sensor.find(host).value();

        if(ip.compare("")!=0){ // ip found

            if(st->is_connected == false){ //
                st->ip = ip;
                st->is_connected = true;
                n_tab->ext_upd(host,NetTable::SC_IP);
                n_tab->ext_upd(host,NetTable::SC_CONN);
            }
            if(!ip_to_host.contains(ip))
                ip_to_host[ip]=host;
        } else { //ip not found
            if(st->is_connected == true){
                st->ip = ip;
                st->is_connected = true;
                n_tab->ext_upd(host,NetTable::SC_IP);
                n_tab->ext_upd(host,NetTable::SC_CONN);
            }
            if(ip_to_host.contains(ip))
                ip_to_host.remove(ip);
        }
    }
    }
}

void MyUDP::udp_rescanst(bool on)
{
    if(on == rescan_timer->isActive()) return;
    if(_is_debug) {
        host_to_sensor["espsensor1.lan"].series_val["mks925"] = 1e-1;
        host_to_sensor["espsensor2.lan"].series_val["imr265:ion"] = 1e-3;
        host_to_sensor["espsensor2.lan"].series_val["imr265:pirani"] = 1e0;
        return;}
    if(on) rescan_timer->start();
    else rescan_timer->stop();
}

void MyUDP::udp_connect(bool state){
    if(state == _is_connected) return;
    _is_connected = state;
    if(state)
        ask_timer->start();
    else
        ask_timer->stop();
    emit is_connected( state);
}

void MyUDP::udp_debug(bool state)
{

    _is_debug = state;
    if(_is_debug){
        rescan_timer->stop();
        ran = new QRandomGenerator64();
        host_to_sensor.clear();
        ip_to_host.clear();

        add("espsensor1.lan","@253PR1?;FF\n",new MKS925C);
        add("espsensor2.lan","A\n",new IMR265);

        host_to_sensor["espsensor1.lan"].series_val["mks925"] = 1e-1;
        host_to_sensor["espsensor2.lan"].series_val["imr265:ion"] = 1e-3;
        host_to_sensor["espsensor2.lan"].series_val["imr265:pirani"] = 1e0;
        udp_rescan();
    } else {
        host_to_sensor.clear();
        ip_to_host.clear();
        add("espsensor1.lan","@253PR1?;FF\n",new MKS925C);
        add("espsensor2.lan","A\n",new IMR265);
        rescan_timer->start();
        }
}
