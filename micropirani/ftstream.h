#ifndef FTSTREAM_H
#define FTSTREAM_H

#include <QObject>
#include <QList>

#include"timevalpoint.h"
class FTStream : public QObject
{
    Q_OBJECT
public:
    explicit FTStream(QObject *parent = nullptr);

signals:
    void refresh(QList<TimeValPoint>* points);
public slots:
    void add(TimeValPoint p);
    void Save();
    void Load();
    void Reset();
private:
    QList<TimeValPoint> points;
};

#endif // FTSTREAM_H
