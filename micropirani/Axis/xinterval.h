#ifndef XINTERVAL_H
#define XINTERVAL_H
#include<QtMath>
#include<QDebug>
#include<QDateTime>
#include<QDataStream>

struct XInterval{

public:

    qint64 max;
    qint64 min;
    qint64 len();
    bool _empty();
    XInterval _offs(qint64 t);
    XInterval _and(XInterval a);
    XInterval _or(XInterval a);
    bool _contain(qint64 t);
    operator QPair<qint64,qint64>();

    friend QDataStream & operator<<(QDataStream &out, const XInterval & s);
    friend QDataStream & operator>>(QDataStream &in, XInterval & s);
    friend QDebug operator<<(QDebug dbg, const XInterval &x);
};

Q_DECLARE_METATYPE(XInterval)

#endif // XINTERVAL_H
