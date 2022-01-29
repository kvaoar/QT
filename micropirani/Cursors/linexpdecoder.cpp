#include "linexpdecoder.h"

linexpdecoder::linexpdecoder()
{

}

linexpdecoder::linexpdecoder(diff2 a, diff2 b):c{a,b}
{
    pt = (c.first.pt + c.second.pt)/2;
    pval = (c.second.pval - c.first.pval)/
            ((c.second.c.second.c.second.pt - c.first.c.first.c.first.pt)/ms); //(Torr/s)/s/s

    Cursor first = c.first.c.first.c.first;
    Cursor c1 = c.first.c.first.c.second;
    Cursor c2 = c.second.c.second.c.first;
    Cursor last = c.second.c.second.c.second;

    //qDebug()<<"EXP a(x-x0) + e^(c*(x-x0)) => b = a*x0; d = e^(c*x0)";
    if((c.second.pval>c.first.pval)&&(c.second.pval<0)) {
        qDebug()<<"EXP y2'' > y1''& y2''<0 => a>0 d<0 c<0 _-*";
        /*
|      ***
|    *
|   *
______________
*/
        _.x0 = first.pt;
        _.y0 = first.pval;
        _.b =  0;
        _.c = log(c.second.pval/c.first.pval)/((c.second.pt-c.first.pt)/ms);
        if(_.c>0) qDebug()<<"EXP c>0 - error";

        /*
(V2 - V0)/(V1 - V0) =  exp(c*(t1-t2))
*/
        qreal v2 = c.second.c.second.pval;
        qreal v1 = c.second.c.first.pval;
        qreal dtv1v2 = +(c.second.c.first.pt - c.second.c.second.pt)/1000.0;
        qreal kV = exp(dtv1v2*abs(_.c));
        //qDebug()<<"v2"<<v2<<"v1"<<v1<<"dt"<<dtv1v2<<"kv"<<kV;
        _.a = (v1*((v2/v1) - kV))/(1-kV);
        if(_.a<0) qDebug()<<"LIN a<0 - error";
        qreal dt = ((last.pt - _.x0)/ms);
        qreal dp = (last.pval - _.y0);
        _.d = (dp - _.a*dt)/(exp(_.c*dt) - 1.0);
        if(_.d>0) qDebug()<<"EXP d>0 - error";
        _.b = _.y0 - calc(first.pt);
        is_valid = true;

        //qDebug()<<" a = "<<_.a<<" b = "<<_.b<<" c = "<<_.c<<" d = "<<_.d<<" x0 = "<<_.x0<<" y0 = "<<_.y0 ;
    }//else
    if((c.second.pval<c.first.pval)&&(c.second.pval>0)){
        qDebug()<<"EXP y2'' < y1'' && y''>0 => a<0 d>0 c<0 *-_";
        /*
| *
|  *
|     ***
____________*/
        _.x0 = first.pt;
        _.y0 = first.pval;
        _.b =  0;
        _.c = log(c.second.pval/c.first.pval)/((c.second.pt-c.first.pt)/ms);
        if(_.c>0) qDebug()<<"EXP c>0 - error";
        /*
(V2 - V0)/(V1 - V0) =  exp(c*(t1-t2))
*/
        qreal v2 = c.second.c.second.pval;
        qreal v1 = c.second.c.first.pval;
        qreal dtv1v2 = +(c.second.c.first.pt - c.second.c.second.pt)/1000.0;
        qreal kV = exp(dtv1v2*abs(_.c));
        //qDebug()<<"v2"<<v2<<"v1"<<v1<<"dt"<<dtv1v2<<"kv"<<kV;
        _.a = (v1*((v2/v1) - kV))/(1-kV);
        if(_.a>0) qDebug()<<"LIN a>0 - error";
        qreal dt = ((last.pt - _.x0)/ms);
        qreal dp = (last.pval - _.y0);
        _.d = (dp - _.a*dt)/(exp(_.c*dt) - 1.0);
        if(_.d<0) qDebug()<<"EXP d<0 - error";
        _.b = _.y0 - calc(first.pt);
        //qDebug()<<" a = "<<_.a<<" b = "<<_.b<<" c = "<<_.c<<" d = "<<_.d<<" x0 = "<<_.x0<<" y0 = "<<_.y0 ;
        is_valid = true;
    } else {
        //qDebug()<<"EXP y2'' or y1'' has different sign";
    }

    //is_valid = ((c1.pval - calc(c1.pt) < 0.3*c1.pval)&&(c2.pval - calc(c2.pt) < 0.3*c2.pval));

}

qreal linexpdecoder::calc(qint64 t){
    qreal _t = (t-_.x0)/ms;
    return  _.b + _.a*_t +_.d*(exp(_.c*_t));
}

void linexpdecoder::replot(QVector<QMap<qint64, qreal> > &add_plot){
    Cursor first = c.first.c.first.c.first;
    Cursor last = c.second.c.second.c.second;
    //c.first.replot(add_plot);
    //c.second.replot(add_plot);
    if(is_valid){
        QMap<qint64, qreal> p;
        int points_cnt = 100;
        qint64 dt = (last.pt - first.pt)/points_cnt;
        p[first.pt] = calc(first.pt);
        for(int i = 1; i < points_cnt-1; i++)
        {
            qint64 t = first.pt + i*dt;
            p[t] = calc(t);
        }
        p[last.pt] = calc(last.pt);
        add_plot.append(p);
    }
}

QDebug operator<<(QDebug dbg, const linexpdecoder &d){
    dbg.nospace()<<"linexpdecoder(a"<<d._.a<<" : b"<<d._.b<<" : c"<<d._.c<<" : d"<<d._.d<<")";
    return dbg.maybeSpace();
}
