#include"cursor.h"

Cursor::Cursor(){
    qDebug()<<"DEFAULT Cursor()";
    _id = -1;}

Cursor::Cursor(FoundP p):FoundP(p){
    qDebug()<<"DEFAULT Cursor()";
}

Cursor::Cursor(qint64 it, QString ser, int i):
    _t(it),series(ser),_id(i){
    _is_abs = (_t > 0);
    _is_sel = false;
    _is_found = false;
    //qDebug()<<"Cursor( qint64 __t,QString ser, int i)"<<*this;
}

Cursor::Cursor(const Cursor &cop):
    FoundP (FoundP(cop)),
     pos(cop.pos),
    _t(cop._t),
    series(cop.series),
    _id(cop._id),
    _is_found(cop._is_found),
    _is_abs(cop._is_abs),
    _is_sel(cop._is_sel)
{}

Cursor::~Cursor()
{
    //qDebug()<<"Cursor destructed";
}

Cursor &Cursor::operator=(const SerIt &in){
    _is_found =true;
    pval = in.value();
    pt = in.key();
    //qDebug()<<"Cursor& operator=(const SerIt & in)"<<in.key()<<in.value();
    return *this;
}

void Cursor::lost(){ _is_found = false; }

void Cursor::grab()
{
    _is_grabbed = true;
}

void Cursor::ungrab()
{
    _is_grabbed = false;
}

void Cursor::found(const SerIt &in)
{
    _is_found =true;
    pval = in.value();
    pt = in.key();
    //qDebug()<<"found SerIt"<<pt<<pval;
}

void Cursor::found(const FoundP &in)
{
    _is_found =true;
    pt = in.pt;
    pval = in.pval;
    //qDebug()<<"foundP"<<pt<<pval;
}

void Cursor::move(qint64 st)
{
    _t = st;
    //qDebug()<<"move"<<_t;
    _is_found = false;
}

qint64 Cursor::t() const
{
    return _t;
}

void Cursor::sel(bool sel){_is_sel = sel;}

bool Cursor::is_found() const{return _is_found;}

bool Cursor::is_grabbed() const
{
    return _is_grabbed;
}

bool Cursor::is_abs() const{return  _is_abs;}

bool Cursor::is_sel() const{return  _is_sel;}

int Cursor::id() const{return _id;}

QString Cursor::ser() const{return series;}

bool operator==(const Cursor &lhs, const Cursor &rhs)
{
    return lhs._id == rhs._id;
}

bool operator<(const Cursor &lhs, const Cursor &rhs)
{
    //qDebug()<<lhs->t<<rhs->t;
    return lhs._t < rhs._t;
}

Cursor &Cursor::operator=(const FoundP &in){
    _is_found =true;
    pt =in.pt;
    pval = in.pval;
    //qDebug()<<"Cursor& operator=(const FoundP & in)"<<in;
    return *this;
}

Cursor &Cursor::operator=(const Cursor &in){
    _is_found =in._is_found;
    _is_abs =in._is_abs;
    _is_sel =in._is_sel;
    pval =in.pval;
    pt = in.pt;
    series =in.series;
    _t=in._t;
    _id =in._id;
    pos = in.pos;
    return *this;
}

QDebug operator<<(QDebug dbg, const Cursor &c){
dbg.nospace()<<
"Cursor("<< FoundP(c)<< c.t() <<c.is_abs()<<")";
//"" fnd:" <<c._is_found<<" abs:"<<c._is_abs;
return dbg.maybeSpace();
};
