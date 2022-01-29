#ifndef DIFF1_H
#define DIFF1_H
#include"foundp.h"
#include "cursor.h"

struct diff1: FoundP{
    QPair<Cursor,Cursor> c;
    const qreal ms = 1000.0;
    diff1();
    diff1(Cursor a, Cursor b);
    qreal calc(qint64 t);
    void replot(QVector<QMap<qint64, qreal> > &add_plot);
    friend QDebug operator<<(QDebug dbg, const diff1 &d);
};
Q_DECLARE_METATYPE(diff1);
#endif // DIFF1_H
