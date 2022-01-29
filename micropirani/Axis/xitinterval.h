#ifndef XITINTERVAL_H
#define XITINTERVAL_H

#include"xinterval.h"
typedef QMap<qint64,qreal>::iterator SerIt;

struct XItInterval{


public:
bool _empty();
operator XInterval();
friend QDebug operator<<(QDebug dbg, const XItInterval &x);
//private:
SerIt max;
SerIt min;

friend QDebug operator<<(QDebug dbg, const SerIt x);
};
Q_DECLARE_METATYPE(XItInterval)

#endif // XITINTERVAL_H
