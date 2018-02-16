#ifndef AXIS_H
#define AXIS_H

#include <QObject>

class axis : public QObject
{
    Q_OBJECT
public:
    explicit axis(QObject *parent = nullptr);

signals:

public slots:
};

#endif // AXIS_H