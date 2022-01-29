#include "yaxis.h"


YAxis::YAxis(){}

YAxis::YAxis(YInterval r)
{

    change(r);
}

YInterval YAxis::range(){return _range;}

const QList<AxeLabel<qreal> > *YAxis::labels() const{return &_labels;}

bool YAxis::is_log() const{return _is_log;}

void YAxis::change(YInterval r){
    _is_log = r.is_log();
    _range.max = qMax(r.max,r.min);
    _range.min = qMin(r.max,r.min);
    if(is_log()) _log_maxmin = log10(_range.max/_range.min);
    //qDebug()<<"YAxis() "<<_is_log<<_range.max<<_range.min;
    refill_labels();
}

bool YAxis::expand(YInterval a){
    bool changed = false;
    if(a.max > _range.max) {_range.max = a.max; changed = true;}
    if(a.min < _range.min) {_range.min = a.min; changed = true;}
    if(changed) refill_labels();
    return changed;
}

qreal YAxis::normalize(qreal physical_val) const{
    if(_is_log)
        return log_normalize(physical_val);
    else
        return lin_normalize(physical_val);
}

qreal YAxis::denormalize(qreal f){
    if(_is_log)
        return log_denormalize(f);
    else
        return lin_denormalize(f);
}

qreal YAxis::lin_normalize(qreal physical_val) const{return (physical_val - _range.min)/(_range.max-_range.min);}

qreal YAxis::log_normalize(qreal physical_val) const{return (log10(physical_val/_range.min)/_log_maxmin);}

qreal YAxis::lin_denormalize(qreal n_val){return (_range.max-_range.min)*n_val + _range.min;}

qreal YAxis::log_denormalize(qreal n_val){return _range.min*pow(10,n_val*_log_maxmin);}

void YAxis::refill_labels(){
    qreal lin_step = 0;
    qreal offs = 0;
    if(_is_log)
        steps = qCeil(_log_maxmin + 1);
    else{

        qreal n = log10(_range.max-_range.min);
        lin_step = pow(10.0,qFloor(n));
        offs = (lin_step*qRound(_range.center()/lin_step))-(5.0*lin_step);
        _range.max = 10.0*lin_step + (offs);
        _range.min = offs;
        steps =qCeil((_range.max-_range.min)/lin_step);
    }
    _labels.clear();
    AxeLabel<qreal> a;
    for(int i = 1; i < steps; i++) {
        if(_is_log)
            a.val = pow(10,qFloor(log10(_range.min))+i);
        else
            a.val = i*lin_step + _range.min;

        if(_is_log)
            a.text.sprintf("%1.0e", a.val);
        else
            a.text = Torr::toString(a.val,qCeil(log10((_range.max)/lin_step)));
        //qDebug()<<"Y "<<a.val<<a.text;

        if(a.val>=0)_labels<< a;
    }
}

QString YAxis::toString(){ return QString("AY: %1 %2 %3 %4").arg(_range.min).arg(_range.max).arg(steps).arg(_is_log?"LOG":"LIN");}

QDebug operator<<(QDebug dbg, const YAxis &x){dbg.nospace()<<"Y"<<x._range<<" steps="<<x.steps<<" log(max/min)="<<x._log_maxmin<<" is_log="<<x._is_log<<" labels="<<x._labels; return dbg.maybeSpace();}

QDataStream &operator>>(QDataStream &in, YAxis &s){in>>s._range>>s.steps>>s._is_log>>s._labels;return in;}

QDataStream &operator<<(QDataStream &out, const YAxis &s){out<<s._range<<s.steps<<s._is_log<<s._labels;return out;}
