#include "xitinterval.h"

bool XItInterval::_empty(){return (max==min);}
XItInterval::operator XInterval(){ return {max.key(),min.key()};}

QDebug operator<<(QDebug dbg, const SerIt x){
    dbg.nospace()<<"("<<QDateTime::fromMSecsSinceEpoch(x.key())
                <<":"<<x.value()<<")"; return dbg.maybeSpace();}

QDebug operator<<(QDebug dbg, const XItInterval &x){
    dbg.nospace()<<"XItInterval("<<QDateTime::fromMSecsSinceEpoch(x.max.key()).toString("mm:ss")<<","<<x.max.value()<<":"
                <<QDateTime::fromMSecsSinceEpoch(x.min.key()).toString("mm:ss")<<","<<x.min.value()<<")"; return dbg.maybeSpace();}
