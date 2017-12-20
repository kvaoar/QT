#ifndef DATATIMESTREAM_H
#define DATATIMESTREAM_H
#include<QList>

class DataTimeStream
{
    QList<QPointF> points;
    int length  = 0;
public:
    DataTimeStream();

    void add(QPointF);
};

#endif // DATATIMESTREAM_H
