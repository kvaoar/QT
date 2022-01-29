#ifndef IMR265_H
#define IMR265_H

#include <QObject>
#include<QtMath>
#include"sparser.h"
class IMR265 : public SParser
{
public:
    IMR265();
    IMR265(IMR265 const &);
    ~IMR265();
    //IMR265():SParser(this,"IMR265",{{"imr265:pirani",{1e+3,1e-2}},{"imr265:ion",{1e+0,1e-6}}})
    QMap<QString, qreal> parser(QByteArray data);
};
Q_DECLARE_METATYPE(IMR265)
Q_DECLARE_METATYPE(IMR265*)
#endif // IMR265_H
