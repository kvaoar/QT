#ifndef MYUDP_H
#define MYUDP_H



#include <QObject>
#include <QUdpSocket>

class MyUDP : public QObject
{
    Q_OBJECT
public:
    explicit MyUDP(QObject *parent = 0);
    void HelloUDP();
signals:

public slots:
    void readyRead();

private:
    QString ESP_ip ;

    QUdpSocket *socket;

};


#endif // MYUDP_H
