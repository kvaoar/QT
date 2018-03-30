#include "glgraph.h"


/**
 * @brief GlGraph::data_init
 * Creating dynamic objects with correct default state
 */
void GlGraph::data_init(){


    time_steps.addItems(QStringList{"1","5","15","30","60"});
    time_steps.setCurrentIndex(0);
    ax_x = new Axis(this,-10.0*minute,0.0,minute,false);
    ax_y = new Axis(this,1.0e-5,1.0e+3,1.0e+1,true);
    cursor_storage = new CursorStorage(this, ax_x, &win_size);
  /*  timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);*/
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
  setFormat(QSurfaceFormat::defaultFormat()); // Двойная буферизация
  glDepthFunc(GL_LEQUAL); // Буфер глубины
  setMouseTracking(true);
}

/**
 * @brief GlGraph::~GlGraph
 * Destructor call delete for all dynamic objects
 */
GlGraph::~GlGraph()
{
    delete ax_x;
    delete ax_y;
   // delete timer;
}



/**
 * @brief GlGraph::vscreen_to_win
 * Convert point from virtual coordinates of data (physical dimensions)
 * to pixel coordinates of QOpenGLWidget screen where left bottom always (0,0).
 * @param p - point in vscreen coordinates
 * @return point in pixel window coordinates
 */
QPointF GlGraph::vscreen_to_win(QPointF p){
QPointF res;
    res.setX(((p.x()-vscreen.x())/vscreen.width())*win_size.width());
    res.setY(((p.y()-vscreen.y())/vscreen.height())*win_size.height());
    return res;
}

/**
 * @brief GlGraph::win_to_vscreen
 * Convert points from pixel coordinates of QOpenGLWidget screen where left
 * bottom always (0,0) to virtual coordinates of data (physical dimensions)
 * @param p - point in in pixel window coordinates
 * @return point in vscreen coordinates
 */
QPointF GlGraph::win_to_vscreen(QPointF p){
    QPointF res;
    res.setX(((p.x()/win_size.width())*vscreen.width())+vscreen.x());
    res.setY(((p.y()/win_size.height())*vscreen.height())+vscreen.y());
    return res;
}

/**
 * @brief GlGraph::refresh_data
 * This slot recieve pointer of new data list every time when data will be
 * updated. Practically it the same pointer, but on this event widget can
 * refresh picture, if regular refreshing disabled.
 * @param pt - data list pointer
 */
void GlGraph::refresh_data(QList<TimeValPoint>* pt){
    points = pt;
    if(data_exist())
        update();
}

void GlGraph::set_ax_y_lin(bool is_lin)
{
    ax_y->set_lin(is_lin);
    if(is_lin){

        float max = ax_y->get_range_max();
        float min = ax_y->get_range_min();
        int exp_max = rint(ceil(log10(max)));
        float p_max = pow(10,exp_max);
        float p_step = p_max / 10.0;

        min = 0;
    ax_y->set_max(p_max);
    ax_y->set_min(min);
    ax_y->set_step(p_step);
    }else{
        ax_y->set_max(1e+3);
        ax_y->set_min(1e-5);
        ax_y->set_step(1.0e+1);
    }
    ax_y->generate_labels();
update();

}

CursorStorage *GlGraph::get_curlist()
{
    return cursor_storage;
}


void  GlGraph::initializeGL()
{
    win_size.setWidth(width());
    win_size.setHeight(height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MULTISAMPLE);
}

void  GlGraph::resizeGL(int w, int h)
{
    win_size.setWidth(w);
    win_size.setHeight(h);
    glViewport((GLint)0, (GLint)0, (GLint)w, (GLint)h);
}

void  GlGraph::paintGL()
{

    QPainter painter;
   painter.begin(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);


    painter.beginNativePainting();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины
    glMatrixMode(GL_PROJECTION); // устанавливаем матрицу
 //   glPushMatrix();
    glLoadIdentity(); // загружаем матрицу

    vscreen.setLeft(ax_x->min());
    vscreen.setRight(ax_x->max());
    vscreen.setBottom(ax_y->min());
    vscreen.setTop(ax_y->max());

    glOrtho(vscreen.left(),vscreen.right(),vscreen.bottom(),vscreen.top(),1,0); // подготавливаем плоскости для матрицы
    //glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //график
    glColor3f(1,0,0);
if(data_exist()){


        qint64 utc_abs_now = points->last().t;
        glBegin(GL_LINE_STRIP);
        ax_y->range_start();
        cursor_storage->start_scan(utc_abs_now,win_to_vscreen(QPointF(mouse_pos.x(),mouse_pos.y())).x());
        for(int i = points->size()-1; i > 0; i--) {
            float ptim = (points->at(i).t - utc_abs_now);
            if(!ax_x->is_ovf(ptim)){
                if(!ax_x->is_unf(ptim)){
                float val = ax_y->val_to_scale(points->at(i).f);
                ax_y->range(points->at(i).f);
                cursor_storage->process_scan(points->at(i));
                glVertex2d(ptim,val);
                }else {
                    i = 0;
                }
            }
        }
        ax_y->range_end();
        cursor_storage->stop_scan();
}
        glEnd();
      glScalef((1.0*win_size.width())/(vscreen.width()),win_size.height()/(vscreen.height()),1.0);
//glPopMatrix();
    //надписи

       painter.endNativePainting();
       //painter.setRenderHint(QPainter::TextAntialiasing);
        plot_axis(&painter);
        if(under_mouse) plot_mouse(&painter);
        plot_cursors(&painter);
        painter.end();


}




void GlGraph::keyPressEvent(QKeyEvent *ke)
{
   /* switch (ke->key())
    {

          case Qt::Key_Space:
                geese_coord();
                break;
    }
    updateGL();*/
}


void GlGraph::mouseMoveEvent(QMouseEvent *me)
{
    // Получаем координаты курсора
    mouse_pos = me->pos();
    under_mouse = true;

    update();

}

void GlGraph::wheelEvent(QWheelEvent *me)
{

   // if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)){
        const int d = me->delta() / (8*15);

        if(ax_x->max()+d*ax_x->step() <= 0)
            ax_x->offset_range(d*ax_x->step());
   // }

    if(QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)){
        const int d = me->delta() / (8*15);
        if(d>0){
            if(time_steps.currentIndex()+d < time_steps.count()){
                time_steps.setCurrentIndex(time_steps.currentIndex()+d);
            }
        }else{
            if(time_steps.currentIndex()+d >= 0){
                time_steps.setCurrentIndex(time_steps.currentIndex()+d);
            }
          }

        ax_x->set_min(ax_x->max() - 10.0*time_steps.currentText().toFloat()*minute);
        ax_x->set_step(minute*time_steps.currentText().toFloat());

    }



    update();
}

void GlGraph::leaveEvent(QEvent *me)
{
    under_mouse = false;
    update();
}

void GlGraph::mousePressEvent(QMouseEvent *me)
{

    //(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))?
    if(me->button()==Qt::LeftButton){
        if(data_exist()){
            cursor_storage->add(mouse_pos,!(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)));
        }
       update();
    }
    if(me->button()==Qt::RightButton){
        if(data_exist()){
            cursor_storage->del(mouse_pos);
        }
       update();

    }
}

void GlGraph::mouseReleaseEvent(QMouseEvent *me)
{

    if(me->button()==Qt::LeftButton){

    }
}



bool GlGraph::data_exist()
{
    if(points != NULL)
        if(points->size()>2)
            return true;
    return false;
}

void GlGraph::plot_mouse(QPainter *painter)
{


    if((data_exist())&&(cursor_storage->mouse->is_found)){

        QFont mouse_cursor_font("Arial", 14, QFont::Bold);
        painter->setPen(QColor(255,255,255,200));
        painter->setBrush(QColor(255,255,255,200));
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter->setFont(mouse_cursor_font);

        qint64 utc_abs_now = points->last().t;
            TimeValPoint p = cursor_storage->mouse->p;
            QPointF screen_data_p = vscreen_to_win(QPointF(p.t-utc_abs_now,ax_y->val_to_scale(p.f)));

            painter->drawLine(mouse_pos.x(),win_size.height(),mouse_pos.x(), 0);
            painter->drawLine(screen_data_p-QPointF(-10,0),screen_data_p-QPointF(+10,0));
            painter->drawLine(screen_data_p-QPointF(0,-10),screen_data_p-QPointF(0,+10));
                //надписи


            QString info;
            info.sprintf("%3.2e", p.f);
            painter->drawText(QPointF(mouse_pos.x(),mouse_pos.y()), info);

    } else{
        painter->drawLine(mouse_pos-QPointF(-10,0),mouse_pos-QPointF(+10,0));
        painter->drawLine(mouse_pos-QPointF(0,-10),mouse_pos-QPointF(0,+10));
    }
}

void GlGraph::plot_cursors(QPainter *painter)
{

    QFont cursor_font("Arial", 12, QFont::Bold);
    painter->setFont(cursor_font);
        painter->setBrush(QColor(255,255,255,200));
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

if(!data_exist()) return;
if(underMouse())
    painter->setPen(QColor(200,200,0,255));
 else
    painter->setPen(QColor(200,200,100,150));

qint64 utc_abs_now = points->last().t;
        foreach (Cursor* c, cursor_storage->cur) {
                if(c->is_abs){
                    if(c->is_found){
                    painter->drawLine(vscreen_to_win(QPointF(c->p.t-utc_abs_now , ax_y->val_to_scale( c->p.f))),
                                     vscreen_to_win(QPointF(c->p.t-utc_abs_now, ax_y->min())));

                    QString info;
                    info.sprintf("%3.2e", c->p.f);
                    painter->drawText(vscreen_to_win(QPointF(c->p.t-utc_abs_now , ax_y->val_to_scale( c->p.f))), info);
                } else{
                    painter->drawLine(c->win_pos(),win_size.height(),c->win_pos(),0);
                }
            }else{
            painter->drawLine(vscreen_to_win(QPointF(c->time_axis_pos, ax_y->val_to_scale( c->p.f))),
                             vscreen_to_win(QPointF(c->time_axis_pos, ax_y->min())));

            QString info;
            info.sprintf("%3.2e", c->p.f);
            painter->drawText(vscreen_to_win(QPointF(c->time_axis_pos, ax_y->val_to_scale( c->p.f))), info);
            }
        }


}

void GlGraph::plot_axis(QPainter *painter)
{

    QFont label_font("Arial", 12, QFont::Bold);
    painter->setPen(QColor(255,255,255,100));
    painter->setBrush(QColor(255,255,255,100));
    painter->setFont(label_font);
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);


for(QList<AxeLabel>::const_iterator i = ax_y->get_labels()->begin(); i != ax_y->get_labels()->end(); ++i){
    painter->drawLine(vscreen_to_win(QPointF(ax_x->min(),i->val)),vscreen_to_win(QPointF(ax_x->max(),i->val)));
    painter->drawText(vscreen_to_win(QPointF(ax_x->min(),i->val))+ QPointF(0,-QFontMetrics(label_font).height())/2 ,i->text);
    }

for(QList<AxeLabel>::const_iterator i = ax_x->get_labels()->begin(); i != ax_x->get_labels()->end(); ++i){
    int m = fabs((i->val)/minute);
    QString t_label; t_label.sprintf("%d:%d",m/60, m%60);
    painter->drawLine(vscreen_to_win(QPointF(i->val,ax_y->min())),vscreen_to_win(QPointF(i->val,ax_y->max())));
    painter->drawText(vscreen_to_win(QPointF(i->val,ax_y->min()))+ QPointF(-4,0) ,t_label);

   }
}




