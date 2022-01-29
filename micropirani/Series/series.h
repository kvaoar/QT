#ifndef SERIES_H
#define SERIES_H

#include<cmath>
#include <QList>
#include<QColor>
#include<QString>
#include<QtMath>
#include<QDebug>
#include<QDateTime>
#include<QJsonObject>
#include<QJsonArray>
#include"Units/torr.h"
#include"Axis/intervals.h"


struct TimeValPoint{
    qint64 t;
    QMap<QString,qreal> f;
    friend QDebug operator<<(QDebug dbg, const TimeValPoint &x){dbg.nospace()<<"("<<x.t<<":"<<x.f<<")"; return dbg.maybeSpace();}
};
Q_DECLARE_METATYPE(TimeValPoint)

class Series{
    public:
    Series(QString nam = "data", QString col = "red", YInterval iy = YInterval{1.0e+3,1.0e-5} );
    ~Series();

    void clear();
    SerIt getAt(qint64 t)const;
    XInterval getAt(XInterval t)const;
    XItInterval getItAt(XInterval t)const;
    YInterval getRange(XItInterval data)const;
    void write(QJsonObject &json) const;
    void read (QJsonObject &json);

    QString name;
    QString color;
    YInterval y;
    XInterval x();
    bool prefer_log = true;
    bool enable = true;
    QMap<qint64,qreal>* physical_values;


    friend QDataStream &operator<<(QDataStream &out, const Series & s);
    friend QDataStream & operator>>(QDataStream &in, Series & s);
    friend QDebug operator<<(QDebug dbg, const Series &x);




};
Q_DECLARE_METATYPE(Series)

#endif // SERIES_H
