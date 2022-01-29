#include "scaler.h"

Scaler::Scaler(QObject *parent) : QObject(parent)
{
    ax = new XAxis({-10*60*1000, 0});
    ay_lin= new YAxis(defaultYlinInterval);
    ay_log= new YAxis(defaultYlogInterval);
    reporter = new QTimer(this);
    reporter->start(10000);
    connect(reporter, &QTimer::timeout, this, &Scaler::ReportStatus);
    setObjectName("Scaler");
}

Scaler::~Scaler()
{
    delete ax;
    delete  ay_lin;
    delete ay_log;
    qDeleteAll(dynrange);
}

void Scaler::setStorage(SeriesStorage *series_stor)
{
    connect(series_stor,
            &SeriesStorage::refresh,
            this,
            &Scaler::ser_onSeriesRefresh
            );
    connect(series_stor,
            &SeriesStorage::last_time
            ,this,
            &Scaler::ser_onLastTimeUpd);
    connect(series_stor,
            &SeriesStorage::append
            ,this,
            &Scaler::ser_onSeriesAppend);
    connect(series_stor,
            &SeriesStorage::color_changed
            ,this,
            &Scaler::ser_onSeriesColorChanged);

}

bool Scaler::is_log() const{
    return log_view;
}

void Scaler::setXComboBox(QComboBox *c)
{
    QStringList l;
    foreach(int dx, zoomT_list){
        if(dx<60)
            l.append(QString("%1s").arg(dx));
        else
            l.append(QString("%1m").arg(dx/60));
    }
    c->addItems(l);
    c->setMaxCount(c->count());
    cbx = c;
    connect(c,SIGNAL(currentIndexChanged(int)), this, SLOT(zoomT(int)));
    connect(this,SIGNAL(toXComboBoxChanged(int)),c,SLOT(setCurrentIndex(int)));
    emit toXComboBoxChanged(2);
}

QStringList Scaler::getSeriesList() const{
    return series.keys();
}

bool Scaler::contains(QString name){
    return series.contains(name);
}

qreal Scaler::normalize_x(qint64 val, qint64 lasttime){
    return ((((val - lasttime) - ax->getRange().min)*1.0)/(ax->len()*1.0));
}

void Scaler::getCurves(QMap<QString, QVector<QPointF> > &out, QSize win_size, const Series* series, XItInterval x)
{
    //qDebug()<<" Scaler::getCurves";

    bool mouse_ser_found = false;
    bool mouse_cur_found = false;
    decimator_result[series->name] = {0,0};
   int total = 0; int accepted = 0;
    out[series->name].clear();
    QPoint prev = {0,0};

    if(x._empty()) return;
    YAxis* y = getY(series->name);
    if(!is_log()){
        YInterval dY = series->getRange(x);//getYRange(series,x);
        if( (y->normalize(dY.max) > 0.9) || (y->normalize(dY.min) < 0.1) ){ y->change(dY); }
    }

        for(CurSeries::iterator cs = rel_cursors[series->name].begin(); cs != rel_cursors[series->name].end(); cs++){
            cs.value().pos.setX(static_cast<int>(win_size.width()*normalize_x(cs.key(),0)));
            cs.value().lost();
            ////qDebug()<<cs.value()<<cs.value().is_found()<<cs.value().pos;
        }
        for(CurSeries::iterator cs = abs_cursors[series->name].begin(); cs != abs_cursors[series->name].end(); cs++){
            QPoint p {static_cast<int>(win_size.width()*normalize_x(cs.key(),x_total.max)), static_cast<int>( win_size.height()*(1.0-y->normalize(cs.value().pval)))};
            cs.value().pos = p;
            if((abs(p.x()-mouse.x())<4)&&(abs(p.y()-mouse.y())<4)){
                emit mouse_curselect(cs.value());
                mouse_cur_found = true;
            }
//            //qDebug()<<cs.value()<<cs.value().is_found()<<cs.value().pos;
        }

     CurSeries::iterator c_it = rel_cursors[series->name].begin();


    for(SerIt p = x.min; p!= x.max; p++){
        qreal time = normalize_x(p.key(),x_total.max);
        qreal tim_win = win_size.width()* time;
        qreal value = 1.0 - y->normalize(p.value());
        qreal value_win = win_size.height()*value;
        QPoint pixel_point = {static_cast<int>(tim_win),static_cast<int>(value_win)};
        total++;

        if( prev.x() != pixel_point.x()){

            bool cur_found = false;
            // search cursors
            while(c_it != rel_cursors[series->name].end()){
                if(c_it->pos.x() < prev.x()) c_it++;
                else break;
            }
            if(c_it != rel_cursors[series->name].end()){
            if(!c_it.value().is_found()){
                    if(c_it->pos.x() <= pixel_point.x()) {
                        c_it->found(p);
                        c_it->pos = pixel_point;
                        cur_found = true;
                        }
                }
            }
            //search mouse

            if((abs(mouse.x() - pixel_point.x())< 4)&&(abs(mouse.y() - pixel_point.y())< 4) ){
                    if(!mouse_ser_found){
                    emit mouse_serselect(series,p.key());
                    mouse_ser_found = true;
                    }
                    if(cur_found&&(!mouse_cur_found)){
                        emit mouse_curselect( *c_it );
                        mouse_cur_found = true;
                    }
                }

            accepted++;
            prev = pixel_point;
            out[series->name].append({ tim_win, value_win });

        }
    }

    if(!mouse_ser_found) emit mouse_serlost(series);
    decimator_result[series->name] = {accepted,total};
    //qDebug()<<"acc"<<accepted<<"tot"<<total;
}

void Scaler::getlabels(QSize win_size, QMap<QString, QList<AxeLabel<int> > > &out)
{
    out.clear();
    getWinNormLabels(out["XAxis"],ax,win_size.width());
    if(is_log()){
    getWinNormLabels(out["YAxis"],getY(),win_size.height());
    }else{
        foreach(QString ser, series.keys()){
            if(series.value(ser)->enable)
                getWinNormLabels(
                            out[ser],
                            getY(ser),
                            win_size.height()
                            );
        }
    }
}

void Scaler::getAxis(AxisLines &axis_lines, AxisLabels &axis_labels)
{
    QMap<QString,QList<AxeLabel<int>>> axes;
    getlabels(win_size,axes);
    axis_lines.clear();
    axis_labels.clear();


    if(axes.contains("XAxis")){
        axis_labels["XAxis"].clear();
        foreach(auto l, axes.value("XAxis")){
            int x = l.val;
            axis_lines.append(QLineF(x,0,x,win_size.height()));
            axis_labels["XAxis"].append({l.text,QPointF(x,win_size.height()-10)});
        }
    }

    if(is_log()){

    if(axes.contains("YAxis")){
        axis_labels["YAxis"].clear();
        foreach(auto l, axes.value("YAxis")){
            int y= win_size.height()- l.val;
            //painter->drawLine(0,y,win_size.width(),y);
            axis_lines.append(QLineF(0,y,win_size.width(),y));
            axis_labels["YAxis"].append({l.text,QPointF(10,y)});
        }

    }

    }else {

    QMap<int,QLineF> horizontal_line;
        foreach(QString ser, getSeriesList() ){
            axis_labels[ser].clear();
            foreach(auto l,axes.value(ser)){
                int y = win_size.height() - l.val;
                horizontal_line[y] = QLineF(0,y,win_size.width(),y);
                axis_labels[ser].append({l.text,QPointF(100*(getSeriesList().indexOf(ser)),y)});
            }
        }
        axis_lines<<horizontal_line.values().toVector();
    }
}

void Scaler::set_status(QString s)
{
    m_status = s;
    emit statusChanged(s);
}

void Scaler::set_lin(bool is_lin)
{

    log_view = !is_lin;
    ////qDebug()<<"is_lin"<<is_lin<<"logview"<<log_view;
    if(is_lin){
        dynrange.clear();
        foreach(QString ser, getSeriesList()){
            const Series* s = series.value(ser);
            XInterval x = (getX()->getRange())._offs(x_total.max);
            YInterval ysignal = s->getRange(s->getItAt(x));
            ////qDebug()<<ser<<s->physical_values->count();
            ////qDebug()<<ysignal.max<<ysignal.min;
            dynrange[ser] = new YAxis(ysignal);
        }
    }

    AskFullRefresh(true);

}

void Scaler::zoomT(int k)
{
    if(zoomT_list.count()>k)
        if(ax->len()>0){
            set_status(QString("zoomx %1").arg(k));
            XInterval r = ax->getRange();
            r.min = r.max-ax->steps()*zoomT_list.at(k)*1000;
            if(r.min < r.max) ax->setRange(r);
            AskFullRefresh(true);
        }
}

void Scaler::zoomTb(bool inc)
{
    int i = cbx->currentIndex()+ ((inc)?(+1):(-1));
    ////qDebug()<<"bool:zoom"<<inc<<i;
    if((i>=0)&&(i<zoomT_list.count()))
    {
        cbx->setCurrentIndex(i);
        emit toXComboBoxChanged(i);
    }
}

void Scaler::offsetT(int len)
{
    qint64 st = len*ax->getStep();
    XInterval r = ax->getRange()._offs(st);
    if(r.max > 0) return;
    // XInterval abst = r._offs(x_total.max);
    //if(abst.min < x_total.min) return;
    ax->setRange(r);
    offsetT_pos += len;
    set_status(QString("OFFSET T [ %1 : %2 ]").arg(ax->getRange().max).arg(ax->getRange().min));
    AskFullRefresh(true);

}


void Scaler::AskFullRefresh(bool send)
{
    foreach(const Series* ser, series.values()) AskSer(ser);
    getAxis(axis_lines,axis_labels);
    emit gl_refresh_axis(&axis_lines,&axis_labels);
    emit gl_refresh_colors(&curve_color);
    if(send) emit gl_plot();
}

void Scaler::AskSer(const Series* ser)
{
    ////qDebug()<<"ASK ser="<<ser<<"from"<<sender();

    curves[ser->name] = {};

        if(ser->enable) {
            if(ser->physical_values->count()!=0){
                XInterval visible = ax->getRange()._offs(x_total.max);
                XItInterval x_it = ser->getItAt(visible);
                getCurves(curves,win_size,ser,x_it);
                getCursors(gcursors,ser,rel_cursors);
                getCursors(gcursors,ser,abs_cursors);
            }
        }
        emit gl_refresh_curves(&curves);
        emit gl_refresh_cursors(&gcursors);
        if(mcurves.count()>0)emit gl_refresh_mcurves(&mcurves);

}

void Scaler::ReportStatus()
{
    QString s;
    for ( auto a = decimator_result.keyValueBegin(); a != decimator_result.keyValueEnd(); a++){
        s += QString("[%1 %2 %3] ").arg(a.base().key()).arg(a.base().value().first).arg(a.base().value().second);
    }
    qDebug()<<s;
    if(!s.isEmpty())set_status(s);

}

void Scaler::getCursors(gCursors &out, const Series* ser,const CurSearch csc)
{

    //qDebug()<<"Scaler::getCursors"<<csc.keys();
    //out.clear();
    if(!csc.contains(ser->name)){
        return;
    }

CurSeries cs = csc.value(ser->name);

if(cs.count() == 0){
    return;
}

QVector<Cursor> cl;

    foreach(Cursor c, cs){
        if(c.is_found()){
            out[c.id()].col = ser->color;
            out[c.id()].text = Torr::toString(c.pval);
            out[c.id()].selected = c.is_sel();
            out[c.id()].text_pos = c.pos;
            out[c.id()].lines.clear();
                    if(c.is_abs()){
                        out[c.id()].lines.append(QLine(c.pos.x()-10, c.pos.y()-10,c.pos.x()+10,c.pos.y()-10));
                        out[c.id()].lines.append(QLine(c.pos.x()-10, c.pos.y()-10,c.pos.x(),c.pos.y()+10));
                        out[c.id()].lines.append(QLine(c.pos.x()+10, c.pos.y()-10,c.pos.x(),c.pos.y()+10));

                    }else {
                        out[c.id()].lines.append(QLine(c.pos.x(), c.pos.y()-10,c.pos.x(),c.pos.y()+10));
                        out[c.id()].lines.append(QLine(c.pos.x()-10, c.pos.y(),c.pos.x()+10,c.pos.y()));
                      }
                    cl.append(c);
            } else {
                out.remove(c.id());
                //qDebug()<<"Scaler::getCursors remove"<<c.id();
            }
        }

    if(cl.count()>0) emit cur_found(cl);
}

void Scaler::ser_onSeriesRefresh(Series *s)
{
    //qDebug()<<"Scaler::onSeriesRefresh(Series *s)";

    if(s->prefer_log)
      ay_log->expand(s->y);
    else
        ay_lin->expand(s->y);

    AskSer(s);

    emit gl_plot();

}


void Scaler::ser_onSeriesAppend(Series *s)
{
    series.insert(s->name, s);
    AskFullRefresh(true);
    emit gl_plot();
}

void Scaler::ser_onSeriesColorChanged(Series *s)
{
    if(!series.contains(s->name)) return;
    curve_color[s->name] = s->color;
    emit gl_refresh_colors(&curve_color);
    emit gl_plot();
}

void Scaler::ser_onLastTimeUpd(XInterval x)
{
    //qDebug()<<"Scaler::onLastTimeUpd(XInterval x)"<<x.max<<x.min;
    x_total = x;


}

void Scaler::cur_onCursorUpdate(QVector<Cursor> *curstor)
{

rel_cursors.clear();
abs_cursors.clear();
gcursors.clear();
for (Cursor* c = curstor->begin(); c < curstor->end(); c++) {
        if(c->is_abs()){
            abs_cursors[c->ser()][c->t()] = *c;
            //qDebug()<<"Scaler::cur_onCursorUpdate add abs"<<*c;
        } else {
            rel_cursors[c->ser()][c->t()] = *c;
            //qDebug()<<"Scaler::cur_onCursorUpdate add rel"<<*c;
        }
    }
//qDebug()<<" Scaler::cur_onCursorUpdate"<<rel_cursors.count();

AskFullRefresh(true);
}

void Scaler::cur_onMetaCursorUpdate(QVector<MetaCursor> *metacursors)
{
    int upd_cnt = 0;
    static bool was_not_empty = false;
qDebug()<<" Scaler::cur_onMetaCursorUpdate"<<metacursors->count();
    if(metacursors == nullptr) return;
    if(metacursors->isEmpty()){
        mcurves.clear();
        if(was_not_empty) emit gl_refresh_mcurves(&mcurves);
        return;}

    was_not_empty = true;
    for(int i = 0; i < metacursors->count(); i++){
        const MetaCursor d = metacursors->at(i);
        QString ser = d.series();
        int mcur_id = d.getID();
        XAxis* ax = getX();
        YAxis* ay = getY(ser);
        mcurves[mcur_id].clear();
        if(!d.isEmpty()){
          for(QVector<QMap<qint64,qreal>>::const_iterator w = d.plot()->begin(); w != d.plot()->end(); w++){
              QVector<QPointF> pict; pict.reserve(w->count());
            for(QMap<qint64,qreal>::const_iterator p = w->begin(); p != w->end(); p++){
                qreal t = ax->normalize(p.key()-x_total.max)*win_size.width();
                qreal v = (1.0 - ay->normalize(p.value()))*win_size.height();
                pict.append(QPointF(t,v));
                }
           // //qDebug()<<pict;
            upd_cnt++;
            mcurves[mcur_id].append(pict);
          }
        }
    }

}

void Scaler::onGlWinsize(QSize win)
{
    //qDebug()<<"void Scaler::onGlWinsize(QSize win)"<<win.width()<<win.height();
    win_size = win;
    AskFullRefresh(true);
}

void Scaler::gl_onMouseMove(QPointF m)
{
    //qDebug()<<"Scaler::mouse_move(QPointF m)";
mouse = {static_cast<int>(m.x()),static_cast<int>(m.y())};
AskFullRefresh(false);
emit mouse_move(m);
}

void Scaler::gl_onAddCursor(QPointF m)
{
    //qDebug()<<"Scaler::add_cursor(QPointF m)";
    emit mouse_addcursor();

}

void Scaler::gl_onGrabCursor(QPointF m)
{
    //qDebug()<<"Scaler::grab_cursor(QPointF m)";
    emit mouse_grabcursor();

}

void Scaler::gl_onDelCursor(QPointF m)
{
    //qDebug()<<"Scaler::del_cursor(QPointF m)";
    emit mouse_delcursor();

}

void Scaler::gl_onInvalidateMouse()
{
    //qDebug()<<"Scaler::invalidate_mouse()";
    m_title.clear();
    emit gl_refresh_mouse(m_title);
    emit mouse_lost();
    emit gl_plot();
}

void Scaler::mouse_onUpd(QMap<QString, FoundP> title)
{

    //qDebug()<<"Scaler::mouse_onUpd("<<title;
    m_title.clear();
    int h = 0;
    for(auto p = title.begin(); p != title.end(); p++){
        QString ser_name = p.key();
        FoundP f = p.value();
        QPointF pos = {win_size.width()*normalize_x(f.pt,x_total.max), h+ win_size.height()*(1.0 - this->getY(ser_name)->normalize(f.pval))};
        m_title.append({ser_name+Torr::toString(f.pval),pos});
        h += 20;
    }
    //qDebug()<<"Scaler::mouse_onUpd(QString title)"<<m_title;
    emit gl_refresh_mouse(m_title);
}

void Scaler::mouse_onMoveGrabbed(QPointF m, Cursor c)
{
    qint64 t = c.is_abs()?(ax->denormalize((m.x()/win_size.width())) + x_total.max):ax->denormalize((m.x()/win_size.width()));
    c.move(t);
    const Series* s = series.find(c.ser()).value();
    c.found(s->getAt(c.t()));
    emit cur_move(c);
}

XAxis *Scaler::getX(){
    return ax;
}

YAxis *Scaler::getY(QString ser){
    if(log_view)
        return ay_log;
    else {
        if(ser ==""){
            return ay_lin;
        }else{
            return dynrange.value(ser);
        }
    }
}

YAxis *Scaler::getYLog(){
    return ay_log;
}

YAxis *Scaler::getYLin(){
    return ay_lin;
}
