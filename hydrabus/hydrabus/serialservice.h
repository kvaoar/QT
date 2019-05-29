#ifndef SERIALSERVICE_H
#define SERIALSERVICE_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include "portselect.h"

class SerialService : public QObject
{
    Q_OBJECT

     Q_PROPERTY(bool connect_state READ connectState WRITE setConnectState NOTIFY connectChanged)
     Q_PROPERTY(QString port_name READ getPort_name() WRITE setPort_name NOTIFY port_nameChanged)

public:
    explicit SerialService(QObject *parent = nullptr);
    bool connectState();


    QString getPort_name() const;
    void setPort_name(const QString &value);

signals:
    void connectChanged(bool st);
    void port_nameChanged(const QString value);
    void error_msg(const QString value);
    void tailChanged(QString value);
    void RX_Line(const QString value);

private slots:
    void handleRead();
    void handleError(QSerialPort::SerialPortError error);
    bool connect_to_port();
    bool disconnect_from_port();

public slots:
    void setConnectState(bool st);
    void serial_send(QString s);
    void rx_timeout();

private:
    bool connected = false;
    QString port_name = nullptr;
    QStringList* rx_lines = nullptr;
    int port_baudrate = QSerialPort::Baud115200;
    QSerialPort::SerialPortError last_error = QSerialPort::SerialPortError::NoError;
    QSerialPort* serialPort = nullptr;
    QString rx_tail;
    QTimer* rx_tim;
};

#endif // SERIALSERVICE_H
