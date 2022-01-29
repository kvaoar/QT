#ifndef YAXIS_H
#define YAXIS_H
#include<QtMath>
#include<QDebug>

#include"Units/torr.h"
#include"yinterval.h"
#include"axis.h"


class YAxis: public Axis<qreal>{
public:
    YAxis();

    YAxis(YInterval r);
    YInterval range();
    const QList<AxeLabel<qreal>>* labels()const;
    bool is_log()const;
    void change(YInterval r);

    bool expand(YInterval a);
    qreal normalize(qreal physical_val)const;
    qreal denormalize(qreal f);
private:
    qreal lin_normalize(qreal physical_val)const;
    qreal log_normalize(qreal physical_val)const;
    qreal lin_denormalize(qreal n_val);
    qreal log_denormalize(qreal n_val);

    YInterval _range; //physical interval
    int steps;
    qreal _log_maxmin;
    bool _is_log;
    QList<AxeLabel<qreal>> _labels;


    void refill_labels();
    QString toString();
   /* bool fromString(QString s) {
        QStringList l = s.split(" ");
        if((l.at(0).compare("AY:") ==0)&&(l.count() == 5)){
            bool res = false;
            _range.min = l.at(1).toDouble(&res);
            if(!res) return false;
            _range.max = l.at(2).toDouble(&res);
            if(!res) return false;
            steps = l.at(3).toInt(&res);
            if(!res) return false;
            is_log = (l.at(4).compare("LOG")==0);
            if((l.at(4).compare("LOG")!=0)&&(l.at(4).compare("LIN")!=0)) return false;
            refill_labels();
            return true;
        }
        return false;
    }*/

    friend QDataStream & operator<<(QDataStream &out, const YAxis & s);
    friend QDataStream & operator>>(QDataStream &in, YAxis & s);
    friend QDebug operator<<(QDebug dbg, const YAxis &x);
};
Q_DECLARE_METATYPE(YAxis)

#endif // YAXIS_H
