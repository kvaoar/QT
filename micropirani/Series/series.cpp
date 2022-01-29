#include"series.h"

Series::Series(QString nam, QString col, YInterval iy){
    name = nam;
    color = col;
    y.max = qMax(iy.max,iy.min);
    y.min = qMin(iy.max,iy.min);
    prefer_log = y.is_log();
    physical_values = new QMap<qint64,qreal>();
}

Series::~Series(){ delete physical_values;}

void Series::clear(){physical_values->clear();}

SerIt Series::getAt(qint64 t) const{return  physical_values->lowerBound(t);}

XInterval Series::getAt(XInterval t) const{return getItAt(t);}

XItInterval Series::getItAt(XInterval t)const{return {physical_values->lowerBound(t.max),physical_values->lowerBound(t.min)};}

YInterval Series::getRange(XItInterval data) const
{
    qreal max = -std::numeric_limits<qreal>::max();
    qreal min = +std::numeric_limits<qreal>::max();
    for(QMap<qint64,qreal>::iterator p = data.min; p!= data.max; p++){
        max = qMax(p.value(),max);
        min = qMin(p.value(),min);
    }
    return  YInterval{max,min};
}

void Series::write(QJsonObject &json) const{
    json["name"] = name;
    json["color"] = color;
    json["y_max"] = y.max;
    json["y_min"] = y.min;
    json["pref_log"] = prefer_log;
    json["enabled"] = enable;
    QJsonObject values;
    for(QMap<qint64,qreal>::iterator p = physical_values->begin(); p != physical_values->end(); p++){
        values.insert(QString::number(p.key()),p.value());
    }
    json["val"] = values;
}

void Series::read(QJsonObject &json){
    if(json.contains("name")) name = json["name"].toString("data");
    color = json["color"].toString("red");
    y.max = json["y_max"].toDouble(y.max);
    y.min = json["y_min"].toDouble(y.min);
    prefer_log = json["pref_log"].toBool(prefer_log);
    enable = json["enabled"].toBool(enable);
    QJsonObject values = json["val"].toObject();
    for(QJsonObject::Iterator i = values.begin(); i != values.end(); i++){
        qint64 t = i.key().toLongLong();
        qreal p = i.value().toDouble(std::nan("parse error"));
        //qDebug()<<t<<p;
        physical_values->insert(t,p);
    }

}

XInterval Series::x(){return {physical_values->firstKey(), physical_values->lastKey()};}

QDebug operator<<(QDebug dbg, const Series &x){
    dbg.nospace()<<"Series color="<<x.color
                <<"enabled"<<x.enable
               <<" dY="<<x.y
              <<" pref_log="<<x.prefer_log
             <<" phys_val.count="
            <<((x.physical_values == nullptr)?("nullptr"):(QString("%1").arg(x.physical_values->count())));
    return dbg.maybeSpace();
}

QDataStream &operator>>(QDataStream &in, Series &s){
    in.setVersion(QDataStream::Qt_5_3);
    in>>s.color>>s.y>>s.prefer_log>>*s.physical_values;
    return in;
}

QDataStream &operator<<(QDataStream &out, const Series &s){
    //out.writeRawData(reinterpret_cast<const char*>(&s), sizeof (s));
    out.setVersion(QDataStream::Qt_5_3);
    out<<s.color<<s.y<<s.prefer_log<<*s.physical_values;
    return out;
}
