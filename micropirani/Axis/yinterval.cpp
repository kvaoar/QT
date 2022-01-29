#include "yinterval.h"

bool YInterval::is_log(){
    if((max<=0)||(min<=0))
        return false;
    else{

        return  (log10(max/min) > 3);
    }
}

qreal YInterval::center(){return (max+min)/2.0;}

qreal YInterval::lin_normalize(qreal physical_val){return (physical_val - min)/(max-min);}

QDataStream &operator<<(QDataStream &out, const YInterval &s){out<<s.max<<s.min;return out;}

QDataStream &operator>>(QDataStream &in, YInterval &s){in>>s.max>>s.min;return in;}

QDebug operator<<(QDebug dbg, const YInterval &x){dbg.nospace()<<"("<<x.max<<":"<<x.min<<")"; return dbg.maybeSpace();}
