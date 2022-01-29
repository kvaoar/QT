#ifndef AXIS_H
#define AXIS_H
#include<QtMath>
#include <QList>
#include<QDebug>

template <class T>
struct AxeLabel{
T val;
QString text;
friend QDataStream & operator<<(QDataStream &out, const AxeLabel<T> & s){out<<s.val<<s.text;return out;}
friend QDataStream & operator>>(QDataStream &in, AxeLabel<T> & s){in>>s.val>>s.text;return in;}
friend QDebug operator<<(QDebug dbg, const AxeLabel<T> &x){dbg.nospace()<<"("<<x.val<<":"<<x.text<<")"; return dbg.maybeSpace();}
};
Q_DECLARE_METATYPE(AxeLabel<qreal>)
Q_DECLARE_METATYPE(AxeLabel<qint64>)

template <typename T> class Axis{
public:
    Axis(){}
    virtual ~Axis(){}
    virtual qreal normalize(T)const{ qDebug()<<"virtual"; return 0;}
    virtual const QList<AxeLabel<T>>* labels()const{qDebug()<<"virtual";return nullptr;}

};
Q_DECLARE_METATYPE(Axis<qreal>)
Q_DECLARE_METATYPE(Axis<qint64>)

#endif // AXIS_H
