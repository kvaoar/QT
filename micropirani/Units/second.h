#ifndef SECOND_H
#define SECOND_H
#include"unit.h"

class Second: public Unit<qint64>
{
public:
    Second();

    static QString toString (qint64 val);
    static const QString unit ;
};

#endif // SECOND_H
