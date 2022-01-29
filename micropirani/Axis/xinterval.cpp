#include "xinterval.h"


qint64 XInterval::len(){return std::abs(max-min);}

bool XInterval::_empty(){return (max <= min);}

XInterval XInterval::_offs(qint64 t){ return{(max + t), (min + t)};}

XInterval XInterval::_and(XInterval a){
    qint64 t1 = qMin(a.max,max);
    qint64 t2 = qMax(a.min,min);
    if(t1>=t2)
        return {t1,t2};
    else
        return {0,0};
}

XInterval XInterval::_or(XInterval a){
    return {qMax(a.max,max),qMin(a.min,min)};
}

bool XInterval::_contain(qint64 t) {return ((t<=max)&&(t>=min));}

QDebug operator<<(QDebug dbg, const XInterval &x){
    dbg.nospace()<<"( max "<<QDateTime::fromMSecsSinceEpoch(x.max)
                <<": min"<<QDateTime::fromMSecsSinceEpoch(x.min)<<")"; return dbg.maybeSpace();}

QDataStream &operator>>(QDataStream &in, XInterval &s){
    in>>s.max>>s.min;
    return in;
}

QDataStream &operator<<(QDataStream &out, const XInterval &s){
    out<< s.max <<s.min;
    return out;
}

XInterval::operator QPair<qint64, qint64>(){ return {max,min};}
