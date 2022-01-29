#ifndef UNIT_H
#define UNIT_H

#include<QString>

template <typename T> class Unit
{
public:
    Unit(){}
static  QString toString(T val);
static  QString toString(T val, int prec);
static const QString unit ;
private:
};

#endif // UNIT_H
