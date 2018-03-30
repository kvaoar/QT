#include "cursor.h"
#include<QDebug>
#include<QDateTime>
Cursor::Cursor()
{
        is_found = false;
        time_axis_pos = 0;
        p.f = 0;
        p.t = 0;
}

Cursor::Cursor(QPointF pos, QSize *window)
{

    win = window;
    fpos = pos.x()/win->width();

    time_axis_pos = 0;
    p.f = 0;
    p.t = 0;
    is_found = false;
}

Cursor::Cursor(QPointF pos, QSize *window, bool c_abs)
{
    win = window;
    fpos = pos.x()/win->width();
    is_abs = c_abs;
    time_axis_pos = 0;
    p.f = 0;
    p.t = 0;
    is_found = false;
}

float Cursor::win_pos()
{
    return fpos*win->width();
}

float Cursor::axe_pos(Axis *axis)
{
    return fpos*(axis->max()-axis->min()) + axis->min();
}

