#ifndef XAXIS_H
#define XAXIS_H

#include"axis.h"
#include"xinterval.h"


class XAxis: public Axis<qint64>{
public:
    XAxis();
    XAxis(XInterval r);
    void setRange(XInterval r);
    XInterval getRange()const;

    const QList<AxeLabel<qint64>>* labels()const;
    int steps()const;
    qint64 len() const;
    QString toString()const;
    qreal normalize(qint64 abs_t)const;
    qint64 denormalize(qreal t)const;
    qint64 getStep()const;

private:

XInterval _range;
    qint64 step;
    QList<AxeLabel<qint64>> _labels;


    void refill_labels();
    /*bool fromString(QString s) {
        QStringList l = s.split(" ");
        if((l.at(0).compare("AX:") ==0)&&(l.count() == 4)){
            bool res = false;
            range.min = l.at(1).toLongLong(&res);
            if(!res) return false;
            range.max = l.at(2).toLongLong(&res);
            if(!res) return false;
            step = l.at(3).toLongLong(&res);
            if(!res) return false;

            refill_labels();
            return true;
        }
        return false;
    }*/

    friend QDataStream & operator<<(QDataStream &out, const XAxis & s);
    friend QDataStream & operator>>(QDataStream &in, XAxis & s);
    friend QDebug operator<<(QDebug dbg, const XAxis &x);
};
Q_DECLARE_METATYPE(XAxis)

#endif // XAXIS_H
