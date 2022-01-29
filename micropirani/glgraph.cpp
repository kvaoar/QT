#include "glgraph.h"
#include<QSurfaceFormat>
#include<QtAlgorithms>
#include<QKeyEvent>
#include<QDebug>


/**
 * @brief GlGraph::data_init
 * Creating dynamic objects with correct default state
 */
void GlGraph::data_init(){

     upd_control_timer = new QTimer(this);
    connect(upd_control_timer, SIGNAL(timeout()), this, SLOT(tick()));
    //upd_control_timer->start(1000);

}

/**
 * @brief GlGraph::GlGraph
 * Constructor call data_init to create dynamic objects and then
 * init OpenGL and switch on mouse tracking
 * @param parent - main window pointer
 */
GlGraph::GlGraph(QWidget* parent):QOpenGLWidget(parent)
{
  data_init();
  QSurfaceFormat fmt;
  fmt.setSamples(16);
  fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  setFormat(fmt); // Двойная буферизация
  setUpdateBehavior(NoPartialUpdate);
  //qDebug()<<"fmt.samples() ="<<fmt.samples();
  //qDebug()<<"fmt.swapInterval() ="<<fmt.swapInterval();
  setMouseTracking(true);

  pcurves = QPen(QColor(255,255,255),2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
  pcurves.setCosmetic(true);
  pcursors = pcurves;
  pcursors.setColor(color(MOUSECUR));
  pcursors.setWidth(3);

  //emit  AskFullRefresh();
}

/**
 * @brief GlGraph::~GlGraph
 * Destructor call delete for all dynamic objects
 */
GlGraph::~GlGraph()
{

}



void GlGraph::refresh_cursors(gCursors *cl)
{
//qDebug()<<"GL UPD cursors"<<cl->count();
   cursors = *cl;
}

void GlGraph::refresh_mcursors(MetaCurves *l)
{
    mcurves = *l;
    qDebug()<<"GL UPD mcursors"<<l->count();

}


void GlGraph::style_changed(QObject *o)
{
    if(o->property("name").isValid()){
        QString name = o->property("name").toString();
        ColorElements index = title.key(name);
        if(o->property("color").isValid())
            if(colormap.contains(index)) colormap[index] =o->property("color").toString();
        if(o->property("font").isValid())
            if(fontmap.contains(index)) fontmap[index] =o->property("font").value<QFont>();
    }
    update();
}

void GlGraph::reset()
{
/*    //qDebug()<<"gl resetted by"<<sender();
    s_stor = nullptr;
    cmouse = nullptr;
    curves.clear();
    curve_color.clear();
    curve_color["XAxis"] = colormap.value(GRID);
    curve_color["YAxis"] = colormap.value(GRID);
    curve_lasttime.clear();

    //axes.clear();
    lasttime = 0;
    cursors=nullptr;*/

}

void GlGraph::gl_plot()
{
    update();
}

void GlGraph::tick()
{

    foreach(QString s, curve_lasttime.keys())
        if(lasttime>curve_lasttime[s])
            if(lasttime-curve_lasttime[s]>upd_control_timer->interval()){
            //qDebug()<<"lasttime ovf ask";
      //      emit AskSer(s);

    }
}

void  GlGraph::initializeGL()
{
    win_size.setWidth(width());
    win_size.setHeight(height());
emit resize_to_scaler(win_size);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины
    glShadeModel(GL_SMOOTH);
    glEnable( GL_MULTISAMPLE );
    glDepthFunc(GL_LEQUAL); // Буфер глубины

}

void  GlGraph::resizeGL(int w, int h)
{
    win_size.setWidth(w);
    win_size.setHeight(h);
    emit resize_to_scaler(win_size);
    update();
}

void  GlGraph::paintGL()
{
QPainter* painter = new QPainter();
painter->begin(this);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
   painter->setRenderHint(QPainter::HighQualityAntialiasing);

   plot_globalscale(painter);
    plot_axis(painter);
    plot_graph(painter);


   plot_cursors(painter);
   if(under_mouse) plot_mouse(painter);
   painter->end();
   delete painter;

}

//one-time on press messages
void GlGraph::keyPressEvent(QKeyEvent *ke)
{
   // qDebug()<<"void GlGraph::keyPressEvent(QKeyEvent *ke)"<<ke;

    if((!key_mod.testFlag(Qt::AltModifier) )&&(ke->modifiers().testFlag(Qt::AltModifier)))emit abs_cursor(true);
//if((!key_mod.testFlag(Qt::ControlModifier) )&&(ke->modifiers().testFlag(Qt::ControlModifier)))

    key_mod = ke->modifiers();
}

//one-time on release messages
void GlGraph::keyReleaseEvent(QKeyEvent *ke)
{
    //qDebug()<<"void GlGraph::keyReleaseEvent(QKeyEvent *ke)"<<ke;
    if((key_mod.testFlag(Qt::AltModifier) )&&(!ke->modifiers().testFlag(Qt::AltModifier)))
        emit abs_cursor(false);

    if((key_mod.testFlag(Qt::ControlModifier) )&&(!ke->modifiers().testFlag(Qt::ControlModifier)))
        emit invalidate_mouse();
    key_mod = ke->modifiers();
}

//every mouse step messages
void GlGraph::mouseMoveEvent(QMouseEvent *me)
{
    if(!under_mouse){
        setFocus(Qt::FocusReason::MouseFocusReason);
        under_mouse = true;
        key_mod = QApplication::keyboardModifiers();
    }

    mouse_pos = me->pos();

    if(key_mod.testFlag(Qt::ControlModifier) ){
        emit mouse_move(mouse_pos);
    } else {
        if(m_title.count()>0) emit invalidate_mouse();
    }

    /*if(cmouse != nullptr)
        if(cmouse->grab_mode()){
            emit mouse_move(mouse_pos);
        }*/

    update();
}

void GlGraph::wheelEvent(QWheelEvent *me)
{
    const int d = -1*me->angleDelta().y()/15;
    qDebug()<<"wheel"<<me->angleDelta().y();
    if(key_mod.testFlag(Qt::ShiftModifier) )
        emit zoomT(d>0);
    else
        emit offsetT(d);
}

void GlGraph::leaveEvent(QEvent *me)
{
    me->setAccepted(true);
    clearFocus();

    key_mod = Qt::NoModifier;
    under_mouse = false;

    emit invalidate_mouse();
    emit abs_cursor(false);
    update();
}

void GlGraph::mousePressEvent(QMouseEvent *me)
{
    if(me->button()==Qt::LeftButton){
        if(key_mod.testFlag(Qt::ShiftModifier))
            emit grab_cursor(mouse_pos);
        else
            emit add_cursor(mouse_pos);
    }
    if(me->button()==Qt::RightButton){
        emit del_cursor(mouse_pos);
    }
}

void GlGraph::mouseReleaseEvent(QMouseEvent *me)
{
me->setAccepted(true);

}

bool GlGraph::data_exist()
{
if(curves.size() == 0)
    return false;
else
    return true;
}

void GlGraph::plot_graph(QPainter *painter)
{
    if(curves.count()==0) return;
    //painter->setBrush(QColor(255,255,255,255));
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    foreach(QString s, curves.keys()){
        QColor col = curve_color.value(s);
        QPen p = pcurves;
        p.setColor(col);
        painter->setPen(p);
        if(dot_plot){
            painter->drawPoints(curves.find(s)->data(),curves.find(s)->count());
        } else {
            painter->drawPolyline(curves.find(s)->data(),curves.find(s)->count());
        }
    }
}

void GlGraph::plot_mouse(QPainter *painter)
{
   // //qDebug()<<"void GlGraph::plot_mouse(QPainter *painter)"<<m_title;
painter->setPen(colormap.value(MOUSECUR));
//painter->setBrush(QColor(255,255,255,255));
QPointF m = mouse_pos;
painter->drawLine(m-QPointF(-10,0),m-QPointF(+10,0));
painter->drawLine(m-QPointF(0,-10),m-QPointF(0,+10));
foreach(auto p, m_title)
painter->drawText(p.second,p.first);
/*if(cmouse != nullptr){
    if(cmouse->found()) painter->drawText(50, 50, cmouse->ser());
    }*/
}

void GlGraph::plot_cursors(QPainter *painter)
{
foreach(CursorGL cgl, cursors){
    ////qDebug()<<cgl.text<<cgl.text_pos;
painter->setPen(cgl.col);
painter->drawLines(cgl.lines.data(),cgl.lines.count());
painter->drawText(cgl.text_pos,cgl.text);
}



if(!mcurves.isEmpty())
    foreach(int id, mcurves.keys()){
            foreach(QVector<QPointF> p, mcurves[id]){
                painter->drawPolyline(p.data(), p.count());
                }
        }
}


void GlGraph::plot_axis(QPainter *painter)
{
    painter->setPen(colormap.value(GRID));
    painter->setFont(fontmap.value(AXELABELS));
    for (AxisLabels::const_iterator p = axis_labels.begin(); p != axis_labels.end(); p++)
    {
        QString ser = p.key();
        if(curve_color.contains(ser)) painter->setPen(curve_color.value(p.key()));
        for(auto pp : p.value()) painter->drawText(pp.second,pp.first);
    }
    painter->drawLines(axis_lines.data(),axis_lines.count());

}

void GlGraph::plot_globalscale(QPainter *painter)
{
   // if(s_stor == nullptr) return;
/*
XInterval tot = s_stor->XTotal();
if(tot._empty()) return;
XInterval ax = s_stor->getX()->getRange()._offs(tot.max);
YInterval win;
QVector<QLineF> lines_screen;
QVector<QLineF> lines_data;


   XInterval tsumm = ax._or(tot);


   win.max = (1.0*(ax.max - tsumm.min))/(1.0*(tsumm.len()));
   win.min = (1.0*(ax.min - tsumm.min))/(1.0*(tsumm.len()));



    QPen p(QColor(0,0,200,255));
    p.setWidth(5);
    painter->setPen(p);
    painter->drawLine(win.max*win_size.width(), win_size.height()-5,win.min*win_size.width(), win_size.height()-5);

    win.max = (1.0*(tot.max - tsumm.min))/(1.0*(tsumm.len()));
    win.min = (1.0*(tot.min - tsumm.min))/(1.0*(tsumm.len()));
    p.setWidth(5);
    p.setColor(QColor("magenta"));
    painter->setPen(p);
    painter->drawLine(win.max*win_size.width(), win_size.height()-10,win.min*win_size.width(), win_size.height()-10);
*/
}

QPointF GlGraph::norm_to_win(QPointF p)
{

    p.setX(p.x()*win_size.width());
    p.setY((1.0-p.y())*win_size.height());

    return p;
}

QPointF GlGraph::win_to_norm(QPoint p){
    QPointF res;
    res.setX((1.0*p.x())/ (1.0*win_size.width()));
    res.setY((1.0*(win_size.height()-p.y()))/ (1.0*win_size.height()));
    return res;
}

bool GlGraph::getActive_upd_control() const
{
    return upd_control_timer->isActive();
}

void GlGraph::setView(QListView *cl)
{
    cl_model = new ColorListModel();
    style_delegate = new StyleEditorDelegate(cl);


    foreach(ColorElements index, title.keys()){
        QObject* obj = new QObject (this);
        obj->setProperty("name", title[index]);
        if(colormap.contains(index)) obj->setProperty("color",colormap[index]);
        if(fontmap.contains(index)) obj->setProperty("font",fontmap[index]);
        cl_model->insert(obj);
    }

    cl->setModel(cl_model);
    cl->setItemDelegate(style_delegate);
    connect(cl_model,SIGNAL(changed(QObject*)),this,SLOT(style_changed(QObject*)));

}

void GlGraph::setScaler(Scaler *scaler)
{

    connect(this, &GlGraph::resize_to_scaler,scaler,&Scaler::onGlWinsize);
    connect(this,&GlGraph::offsetT,scaler,&Scaler::offsetT);
    connect(this, &GlGraph::zoomT,scaler, &Scaler::zoomTb );

    connect(this, &GlGraph::mouse_move,scaler,&Scaler::gl_onMouseMove);
    connect(this,&GlGraph::add_cursor,scaler,&Scaler::gl_onAddCursor);
    connect(this, &GlGraph::grab_cursor,scaler, &Scaler::gl_onGrabCursor );
    connect(this, &GlGraph::del_cursor,scaler, &Scaler::gl_onDelCursor );
    connect(this, &GlGraph::invalidate_mouse,scaler, &Scaler::gl_onInvalidateMouse);

        //connect(this, &GlGraph::abs_cursor,scaler, &Scaler::gl_onAbsMouse);

    connect(scaler, &Scaler::gl_refresh_curves, this, &GlGraph::refresh_curves);
    connect(scaler, &Scaler::gl_refresh_axis, this, &GlGraph::refresh_axis);
    connect(scaler, &Scaler::gl_refresh_colors, this, &GlGraph::refresh_colors);
    connect(scaler, &Scaler::gl_refresh_cursors, this, &GlGraph::refresh_cursors);
    connect(scaler, &Scaler::gl_refresh_mcurves, this, &GlGraph::refresh_mcursors);
    connect(scaler, &Scaler::gl_refresh_mouse, this, &GlGraph::refresh_mouse);


    connect(scaler, &Scaler::gl_plot, this, &GlGraph::gl_plot);

}

void GlGraph::setActive_upd_control(bool value)
{
    value?upd_control_timer->start():upd_control_timer->stop();
}

void GlGraph::refresh_curves(SerCurves *_curves)
{
    //qDebug()<<"GL UPD curves";
curves = *_curves;

}

void GlGraph::refresh_axis(AxisLines *_axis_lines, AxisLabels *_axis_labels)
{
        //qDebug()<<"GL UPD axis";
    axis_lines = *_axis_lines;
    axis_labels = *_axis_labels;
}

void GlGraph::refresh_colors(SerColors *col)
{
        //qDebug()<<"GL UPD colors";
        curve_color = *col;
}

void GlGraph::refresh_mouse(MouseTitle mtit)
{
    //qDebug()<<"GL UPD mouse";
m_title = mtit;
}

int GlGraph::getUpdate_lag_ms() const
{
    return upd_control_timer->interval();
}

void GlGraph::setUpdate_lag_ms(int value)
{
    upd_control_timer->setInterval(value);
    update();

}

