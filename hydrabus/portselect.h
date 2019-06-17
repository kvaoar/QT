#ifndef PORTSELECT_H
#define PORTSELECT_H

#include <QDialog>
#include <QSerialPortInfo>

namespace Ui {
class PortSelect;
}

class PortSelect : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(QString port_name READ port_name WRITE setPortName)
public:
    explicit PortSelect(QWidget *parent = nullptr);
    ~PortSelect();
    QString port_name();

public slots:
    void setPortName(QString s);

private:
    Ui::PortSelect *ui;
        QSerialPortInfo sp_info;
        QString __port_name;

};

#endif // PORTSELECT_H
