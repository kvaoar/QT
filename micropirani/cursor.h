#ifndef CURSOR_H
#define CURSOR_H
#include"timevalpoint.h"
#include<QPointF>
#include<QSize>
#include"axis.h"

class Cursor
{
public:
    Cursor();
    Cursor(QPointF pos, QSize *window);
    Cursor(QPointF pos, QSize *window, bool c_abs);
    TimeValPoint p = TimeValPoint{0,0};
    bool is_found = false;
    bool is_sel = false;
    bool is_abs = true;
    qint64 time_axis_pos = 0;
    float win_pos();
    float axe_pos(Axis* axis);
    float fpos;
private:

QSize *win;

};

#endif // CURSOR_H
