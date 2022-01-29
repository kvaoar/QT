#include "diff1.h"


diff1::diff1()
{

}

diff1::diff1(Cursor a, Cursor b):c{a,b}
{
    pt = (c.first.pt + c.second.pt)/2;
    pval = (c.second.pval - c.first.pval)/((c.second.pt - c.first.pt)/ms);//Torr/s
}

qreal diff1::calc(qint64 t){
    qreal dt = (t - c.first.pt)/ms;
    //qDebug()<<"t"<<t<<"t0"<<c[1].pt<<"dt"<<dt<<"sec"<<dt/1000.0;
    //qDebug()<<"dy"<<dy<<"pval"<<found.pval<<"dy+y0"<<dy + c[1].pval;
    return  c.first.pval + pval*dt;
}

void diff1::replot(QVector<QMap<qint64, qreal> > &add_plot){
    QMap<qint64, qreal> p;
    p[c.first.pt] = calc(c.first.pt);
    p[c.second.pt] = calc(c.second.pt);
    add_plot.append(p);
}

QDebug operator<<(QDebug dbg, const diff1 &d){
    dbg.nospace()<<" diff1("<<d.c<<")  => {"<<d.pt<<"t; "<<d.pval<<"p}";
    return dbg.maybeSpace();
}
