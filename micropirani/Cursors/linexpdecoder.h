#ifndef LINEXPDECODER_H
#define LINEXPDECODER_H
#include"cursor.h"
#include"diff2.h"

/*
* y = ax+b + e^(cx+d)
*/
struct linexpdecoder:FoundP{
QPair<diff2 ,diff2 > c;
    struct{
        qint64 x0 = 0;
        qreal y0 = 0;
    qreal a =0;
    qreal b =0;
    qreal c =0;
    qreal d =0;
    } _;
    const qreal ms = 1000.0;
    bool is_valid = false;

    linexpdecoder();
    linexpdecoder(diff2 a, diff2 b);

qreal calc(qint64 t);
void replot(QVector<QMap<qint64, qreal>>& add_plot);
friend QDebug operator<<(QDebug dbg, const linexpdecoder &d);
};

Q_DECLARE_METATYPE(linexpdecoder)

#endif // LINEXPDECODER_H
