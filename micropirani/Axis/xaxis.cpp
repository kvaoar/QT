#include "xaxis.h"
#include"Units/second.h"
XAxis::XAxis(){}

XAxis::XAxis(XInterval r){
    setRange(r);

}

void XAxis::setRange(XInterval r){
    _range ={qMax(r.max,r.min), qMin(r.max,r.min)};
    step = r.len()/steps();
    //qDebug()<<"step"<<step<<"max"<<_range.max<<"min"<<_range.min;
    refill_labels();
}

XInterval XAxis::getRange() const{return _range;}

const QList<AxeLabel<qint64> > *XAxis::labels() const{return &_labels;}

int XAxis::steps() const{return 10;}

qint64 XAxis::len() const {return _range.max - _range.min;}

QString XAxis::toString() const{ return QString("AX: %1 %2 %3").arg(_range.min).arg(_range.max).arg(step);}

qreal XAxis::normalize(qint64 abs_t) const{return (((abs_t - _range.min)*1.0)/(len()*1.0)); }

qint64 XAxis::denormalize(qreal t) const{return static_cast<qint64>(t*len()) + _range.min; }

qint64 XAxis::getStep() const{return step;}

void XAxis::refill_labels(){
    _labels.clear();
    AxeLabel<qint64> a;
    int cnt = steps();
    for(int i = 0; i < cnt ; i++) {
        a.val = _range.min+((i)*step);
        int s = static_cast<int>(std::abs(((a.val)/1000)%60));
        int m = static_cast<int>(std::abs(((a.val)/(60*1000))%60));
        int h = static_cast<int>(std::abs((a.val)/(60*60*1000)));
        if(len()>=(10*60*1000))
            a.text.sprintf("%d:%d",h, m);
        else
            a.text.sprintf("%d:%d:%d",h,m, s);
        _labels<< a;
    }
}

QDebug operator<<(QDebug dbg, const XAxis &x){dbg.nospace()<<"X"<<x._range<<" step="<<x.step<<" labels="<<x._labels; return dbg.maybeSpace();}

QDataStream &operator>>(QDataStream &in, XAxis &s){in>>s._range>>s.step>>s._labels;return in;}

QDataStream &operator<<(QDataStream &out, const XAxis &s){out<<s._range<<s.step<<s._labels;return out;}
