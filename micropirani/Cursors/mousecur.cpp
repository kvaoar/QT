#include"mousecur.h"

/*CursorM::CursorM(){
    qDebug()<<"Default constructor!!";
}*/

MouseCur::MouseCur(QObject *parent): QObject(parent){
}

void MouseCur::setScaler(Scaler *scal)
{
    connect(scal,&Scaler::mouse_lost,this,  &MouseCur::lost);

    connect(scal,&Scaler::mouse_serselect,this,  &MouseCur::serselect);
    connect(scal,&Scaler::mouse_serlost,this,  &MouseCur::serlost);
    connect(scal,&Scaler::mouse_curselect,this,  &MouseCur::curselect);
    connect(scal,&Scaler::mouse_move,this,  &MouseCur::move_mouse);

    connect(scal,&Scaler::mouse_addcursor,this,  &MouseCur::add_cursor);
    connect(scal,&Scaler::mouse_grabcursor,this,  &MouseCur::grab_cursor);
    connect(scal,&Scaler::mouse_delcursor,this,  &MouseCur::del_cursor);
    connect(this, &MouseCur::refresh_p, scal, &Scaler::mouse_onUpd);
    connect(this, &MouseCur::move_grabbed, scal, &Scaler::mouse_onMoveGrabbed);
}

bool MouseCur::found_unic() const{
    return (is_found&&(found_series.count()==1));
}

bool MouseCur::found() const{
    return is_found;
}


bool MouseCur::grab_mode() const{
    return grabbed;
}


void MouseCur::drop_grabbed(){
    grabbed = false;
}


void MouseCur::move_mouse(QPointF mouse){
    if(grabbed)
        emit move_grabbed(mouse, grabbed_cur);
}

void MouseCur::add_cursor()
{
    if(found_unic()){
        if(found_cur.isEmpty()){
        QString ser = found_series.firstKey();
        emit newcursor(ser,found_series.first(),is_abs||_tmp_abs);
        emit lost();
        }
    }
}

void MouseCur::grab_cursor()
{
    if(!found_cur.isEmpty()){
        qDebug()<<found_cur.first();
        grabbed_cur = found_cur.first();
        grabbed = true;
    }
}

void MouseCur::del_cursor()
{
    if(!found_cur.isEmpty()){
        emit delcursor(found_cur.first());
        emit lost();
    }
}

void MouseCur::abs_cursor(bool _is_abs)
{
    QObject* senderObj = sender();
    if(senderObj->objectName() == "openGLWidget") _tmp_abs = _is_abs;
    else is_abs = _is_abs;
     emit abs_changed(is_abs);
}

void MouseCur::curselect(Cursor c)
{
    found_cur[c.ser()] = c;
}

void MouseCur::serselect(const Series *ser, qint64 t)
{
    FoundP p {t,ser->physical_values->value(t) };
        found_series[ser->name] = p;
        is_found = true;
        emit refresh_p(found_series);
}

void MouseCur::serlost(const Series *ser)
{
    if(found_series.contains(ser->name)){
        found_series.remove(ser->name);
        if(found_cur.contains(ser->name))found_cur.remove(ser->name);
        if(found_series.count()==0) is_found = false;
        emit refresh_p(found_series);
    }
}

void MouseCur::lost()
{
    is_found = false;
    grabbed =false;
    found_cur.clear();
    found_series.clear();
    emit refresh_p(found_series);

}

