#ifndef SENSORSTATE_H
#define SENSORSTATE_H

#include <QObject>
#include <QHostInfo>
#include<QtMath>
#include"Parsers/sparser.h"
typedef  QMap<QString,qreal> HostValues;
Q_DECLARE_METATYPE(HostValues)
typedef QMap<QString, QPair<qreal,qreal>> HostRanges;
Q_DECLARE_METATYPE(HostRanges)
//typedef HostValues (*parser)(QByteArray data);




struct SensorState{
    QString host;
    QString ip;
    QString ask;
    qint64 last_time;
    HostValues series_val;
    HostRanges series_ranges;
    bool is_connected;
    SParser* p;
    friend QDebug operator<<(QDebug dbg, const SensorState &x);
};

Q_DECLARE_METATYPE(SensorState)
#endif // SENSORSTATE_H
