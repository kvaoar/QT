#ifndef CURSOR_H
#define CURSOR_H
#include<QtMath>
#include<QDebug>
#include<QPoint>
#include"Units/torr.h"
#include"Series/series.h"
#include"foundp.h"



class Cursor : public FoundP
{

public:
    Cursor();
    Cursor(FoundP p);
    Cursor( qint64 it,QString ser, int i);
    Cursor(const Cursor &cop);
    ~Cursor();

    Cursor(Cursor&&) = default;

    Cursor& operator=(const Cursor & in);
    Cursor &operator=(const FoundP & in);
    Cursor& operator=(const SerIt & in);

    void lost();
    void grab();
    void ungrab();
    void found(const SerIt & in);
    void found(const FoundP & in);
    void move(qint64 st);
    qint64 t()const;
    void sel(bool sel);

    bool is_found()const;
    bool is_grabbed()const;
    bool is_abs()const;
    bool is_sel()const;
    int id()const;
    QString ser() const;


    QPoint pos;

//FoundP found;
private:
    volatile qint64 _t;
    QString series;
    int _id;
    volatile bool _is_found;
    volatile bool _is_grabbed;
   volatile bool _is_abs;
   volatile bool _is_sel;

    friend bool operator<(const Cursor& lhs, const Cursor& rhs);
    friend bool operator==(const Cursor& lhs, const Cursor& rhs);

friend QDebug operator<<(QDebug dbg, const Cursor &c);
};


Q_DECLARE_METATYPE(Cursor)
Q_DECLARE_METATYPE(Cursor*)


/*
typedef QMap<qint64, Cursor*>::iterator CurIt ;
typedef QPair<CurIt,CurIt> CurItInterval;*/


#endif
