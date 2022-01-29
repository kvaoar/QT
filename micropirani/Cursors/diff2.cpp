#include "diff2.h"


diff2::diff2()
{

}

diff2::diff2(diff1 a, diff1 b):c{a,b}
{
    pt = (c.first.pt + c.second.pt)/2;
    pval = (c.second.pval - c.first.pval)/((c.second.pt - c.first.pt)/ms);//(Torr/s)/s
    Cursor c2 = c.first.c.second;
    qint64 x0i = c.first.pt;
    qreal y0 = c.first.calc(x0i);
    qreal y1 = c2.pval - y0;
    qreal x1 = ((c2.pt-x0i)/ms);
    qint64 x2i = (c.second.pt);
    qreal  x2 = (x2i - x0i)/ms;
    qreal y2 = c.second.calc(x2i) - y0;
    //qDebug()<< x0i << y0 << " _ "<<x1<<y1<<"  _ "<<x2<<y2;
    qreal bdiva = (y2*pow(x1,2) - y1*pow(x2,2))/((x2*y1) - (x1*y2));
    k.a = y1/(x1*x1 + bdiva*x1);
    k.b = k.a*bdiva;
    k.c = y0;
}

qreal diff2::calc(qint64 t){
    qint64 x0i = c.first.pt;
    qreal _t = (t - x0i)/ms;
    qreal val =  k.a*_t*_t + k.b*_t + k.c;
    return  val;
}

void diff2::replot(QVector<QMap<qint64, qreal> > &add_plot){
    Cursor first = c.first.c.first;
    Cursor last = c.second.c.second;
    //c.first.replot(add_plot);
    //c.second.replot(add_plot);
    QMap<qint64, qreal> p;
    int point_cnt = 100;
    qint64 dt = (c.second.pt - c.first.pt)/point_cnt;
    p[c.first.pt] = calc(c.first.pt);
    for(int i = 1; i < point_cnt-1; i++)
    {
        qint64 t = c.first.pt + i*dt;
        p[t] = calc(t);
    }
    p[c.second.pt] = calc(c.second.pt);

    qreal endval = c.second.calc(c.second.pt);
    if(abs(p.last()-endval) > 0.01*endval) qDebug()<<p.firstKey()<<p.first()<<"="<<c.first.calc(c.first.pt)<<"  -  "<<p.lastKey()<<p.last()<<"="<<c.second.calc(c.second.pt);
    add_plot.append(p);
}

QDebug operator<<(QDebug dbg, const diff2 &d){
    dbg.nospace()<<" diff2("<<d.c<<") => {"<<d.pt<<"t; "<<d.pval<<"p}";
    return dbg.maybeSpace();
}
