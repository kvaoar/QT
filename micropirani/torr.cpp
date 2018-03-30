#include "torr.h"

Torr::Torr()
{

}

QString Torr::toString(float val)
{

    QString s;
    if(val >= 1e+0)
        s.sprintf("%g Torr",val);
    else
        if(val >= 1e-3) //m
            s.sprintf("%g mTorr",val*1e+3);
        else
            if(val >= 1e-6) //u
                s.sprintf("%g uTorr",val*1e+6);
            else
                s.sprintf("%g nTorr",val*1e+9);
return s;

}
