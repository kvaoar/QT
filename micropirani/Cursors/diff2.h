#ifndef DIFF2_H
#define DIFF2_H
#include"cursor.h"
#include"diff1.h"

struct diff2:FoundP{
QPair<diff1 ,diff1 > c;
const qreal ms = 1000.0;
struct{
qreal a =0;
qreal b =0;
qreal c =0;
} k;
    diff2();
    diff2(diff1 a, diff1 b);
    qreal calc(qint64 t);
    void replot(QVector<QMap<qint64, qreal> > &add_plot);
    friend QDebug operator<<(QDebug dbg, const diff2 &d);
};
Q_DECLARE_METATYPE(diff2);

#endif // DIFF2_H
