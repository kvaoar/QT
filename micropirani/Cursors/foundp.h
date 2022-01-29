#ifndef FOUNDP_H
#define FOUNDP_H
#include<QtMath>
#include<QDebug>
struct FoundP{
    qint64 pt;
    qreal pval;
    friend QDebug operator<<(QDebug dbg, const FoundP &d);
};
Q_DECLARE_METATYPE(FoundP)

#endif // FOUNDP_H
