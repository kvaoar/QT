#ifndef SERIALSERVICE_H
#define SERIALSERVICE_H

#include <QObject>

class SerialService : public QObject
{
    Q_OBJECT
public:
    explicit SerialService(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SERIALSERVICE_H