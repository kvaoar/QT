#ifndef MYUDP_H
#define MYUDP_H

#include <QObject>
#include <QUdpSocket>
#include<QDateTime>

class MyUDP : public QObject
{
    Q_OBJECT
public:
    explicit MyUDP(QObject *parent = 0);
signals:
    void udp_recieved(QByteArray data, QDateTime time);
    void is_connected(bool state);
public slots:
    void udp_connect(bool state);
    void udp_ask();
    void udp_read();
private:
    QString ESP_ip ;
    QHostAddress sender;
    quint16 senderPort;
    QUdpSocket *socket;
    bool _is_connected;
};

#endif
