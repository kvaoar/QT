#include "imr265.h"

#define ADC_MAX (65535)
#define VIN_MAX (9.87)
#define ION_MAX (7.5)
#define ION_MIN (1.5)
#define ION_TORR (7.625)
#define PIRANI_MAX (9.75)
#define PIRANI_MIN (8.5)
#define PIRANI_TORR (9.031)

IMR265::IMR265()
    :SParser("IMR265",{{"imr265:pirani",{1e+3,1e-2}},{"imr265:ion",{1e+0,1e-6}}})
{

}

IMR265::IMR265(const IMR265 &)
:SParser("IMR265",{{"imr265:pirani",{1e+3,1e-2}},{"imr265:ion",{1e+0,1e-6}}})
{

}

IMR265::~IMR265()
{
 qDebug()<<"IMR265::~IMR265()";
}

QMap<QString, qreal> IMR265::parser(QByteArray data)
{
   // qDebug()<<"parser IMR265::parser(QByteArray data)";
        QString s =  QString::fromStdString(data.data());
        s = s.toLower().mid(1,s.length()-1);
        bool ok(false);
        int f = s.toInt(&ok);

        qreal vin = (f*VIN_MAX)/(1.0*ADC_MAX);

        qreal res = std::nan("range");

        if((vin> ION_MIN)&&(vin < ION_MAX)){
            res = pow(10,(vin-ION_TORR));
            return {{"imr265:ion",res}};
        }

        if((vin> PIRANI_MIN)&&(vin < PIRANI_MAX)){
            res = pow(10,4.0*(vin-PIRANI_TORR));
            return {{"imr265:pirani",res}};
        }





        return {{"imr265:pirani",std::nan("len")}};
}
