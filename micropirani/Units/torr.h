#ifndef TORR_H
#define TORR_H
#include<QObject>
#include<QString>
#include"unit.h"

class Torr: public Unit<qreal>
{
public:
    Torr();
static  QString toString(qreal val);
static  QString toString(qreal val, int prec);
static const QString unit ;
private:
};



Q_DECLARE_METATYPE(Torr)
#endif
