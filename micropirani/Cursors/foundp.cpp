#include "foundp.h"


QDebug operator<<(QDebug dbg, const FoundP &d){
    dbg.nospace()<<" FoundP("<<d.pt<<" : "<<d.pval<<")";
    return dbg.maybeSpace();
}
