#include"sensorstate.h"

QDebug operator<<(QDebug dbg, const SensorState &x){
    dbg.nospace()<<"SensorState(x.host="<<x.host
                <<" ip="<<x.ip
               <<" ask="<<x.ask
              <<" last_time="<<x.last_time
             <<" series_val="<<x.series_val
            <<" series_ranges="<<x.series_ranges
           <<" is_connected="<<x.is_connected;
    if(x.p == nullptr) dbg.nospace()<<"nullptr"; else dbg.nospace()<<*x.p;
    dbg.nospace()<<")";


    return dbg.maybeSpace();}
