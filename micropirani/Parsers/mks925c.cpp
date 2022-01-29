#include "mks925c.h"

MKS925C::MKS925C()
:SParser("MKS925C",{{"mks925",{1e+3,1e-5}}})
{

}

MKS925C::MKS925C(const MKS925C &)
:SParser("MKS925C",{{"mks925",{1e+3,1e-5}}})
{

}

MKS925C::~MKS925C()
{
qDebug()<<"IMR265::~IMR265()";
}

QMap<QString, qreal> MKS925C::parser(QByteArray data)
{
   // qDebug()<<" MKS925C::parser(QByteArray data)";
    const int PACK_SIZ = 17;
    const int FLOAT_POS =7;
    const int FLOAT_LEN =7;
        QString s =  QString::fromStdString(data.data());

            if(s.size() == PACK_SIZ){
                s = s.toLower().mid(FLOAT_POS,FLOAT_LEN);
                bool ok(false);
                double f = s.toDouble(&ok);
                if(ok){
                     return {{"mks925",f}};

                }else{
                    return {{"mks925",std::nan("parse")}};
                }
            }else {
                return {{"mks925",std::nan("len")}};
            }
}
