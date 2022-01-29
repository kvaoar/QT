#include "second.h"
#include<QDebug>
const QString Second::unit = "sec";

Second::Second():Unit<qint64>()
{

}

QString Second::toString(qint64 val)
{
    int ms = static_cast<int>(std::abs(((val)%1000)));
    int s = static_cast<int>(std::abs(((val)/1000)%60));
    int m = static_cast<int>(std::abs(((val)/(60*1000))%60));
    int h = static_cast<int>(std::abs((val)/(60*60*1000)));
    int d = static_cast<int>(std::abs(((val)/(24*60*60*1000))%(60*60)));
    qDebug()<<val<<" = "<<d<<h<<m<<s<<ms;

    if(val < 1e3){
        return QString::number(ms)+" m" +unit;

    } else if (val < 60*1e3) {
        return QString::number((val)/1000.0,'g',3) +unit;
    }else if (val < 60*60*1e3) {
        return QString("%1:%2").arg(m).arg(s);
    } else if (val < 24*60*60*1e3) {
        return QString("%1:%2:%3").arg(h).arg(m).arg(s);
    }else {
        return QString("%1d %2:%3:%4").arg(d).arg(h).arg(m).arg(s);
    }

}
