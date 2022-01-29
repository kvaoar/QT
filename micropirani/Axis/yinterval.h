#ifndef YINTERVAL_H
#define YINTERVAL_H

#include<QtMath>
#include<QDebug>
#include<QDateTime>
#include<QDataStream>

struct YInterval{
    qreal max;
    qreal min;
    bool is_log();

    qreal center();
    qreal lin_normalize(qreal physical_val);
    friend QDataStream & operator<<(QDataStream &out, const YInterval & s);
    friend QDataStream & operator>>(QDataStream &in, YInterval & s);
    friend QDebug operator<<(QDebug dbg, const YInterval &x);
};
Q_DECLARE_METATYPE(YInterval)

#endif // YINTERVAL_H
