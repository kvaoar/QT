#include "torr.h"
#include<QDebug>
const QString Torr::unit = "Torr";

Torr::Torr():Unit<qreal>()
{

}

QString Torr::toString(qreal val)
{

    //QString s;
    if(val >= 1e+0)
        return  QString::number(val, 'g',3)+" " +unit;
        //s.sprintf("%g Torr",val);
    else
        if(val >= 1e-3) //m
            return  QString::number(val*1e+3, 'g',3)+" " +"m"+unit;
           // s.sprintf("%1.00e mTorr",val*1e+3);
        else
            if(val >= 1e-6) //u
                return  QString::number(val*1e+6, 'g',3)+" " +"u"+unit;
                //s.sprintf("%e uTorr",val*1e+6);
            else
                return  QString::number(val*1e+6, 'g',2)+" " +"n"+unit;
                //s.sprintf("%e nTorr",val*1e+9);
//return s;

}

QString Torr::toString(qreal val, int prec)
{

   // QString s;
    if(val >= 1e+0)
        return  QString::number(val, 'g',prec)+" " +unit;
        //s.sprintf("%g Torr",val);
    else
        if(val >= 1e-3) //m
            return  QString::number(val*1e+3, 'g',prec)+" " +"m"+unit;
           // s.sprintf("%1.00e mTorr",val*1e+3);
        else
            if(val >= 1e-6) //u
                return  QString::number(val*1e+6, 'g',prec)+" " +"u"+unit;
                //s.sprintf("%e uTorr",val*1e+6);
            else
                return  QString::number(val*1e+6, 'g',prec)+" " +"n"+unit;
                //s.sprintf("%e nTorr",val*1e+9);

    //qDebug()<<val<<prec<<s;
//return s;
}
