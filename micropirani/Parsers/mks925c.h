#ifndef MKS925C_H
#define MKS925C_H

#include <QObject>
#include"sparser.h"

class MKS925C : public SParser
{
public:
    MKS925C();
    MKS925C(MKS925C const &);
    ~MKS925C();
    //MKS925C():SParser(this,"MKS925C",{{"mks925",{1e+3,1e-5}}})
    QMap<QString, qreal> parser(QByteArray data);
};
Q_DECLARE_METATYPE(MKS925C)
Q_DECLARE_METATYPE(MKS925C*)
#endif // MKS925C_H
