#include "glgraph.h"


void GlGraph::data_init(){
   //
   // win_size = new QSize(0,0);

}


GlGraph::GlGraph():QOpenGLWidget()
{
 //  data_init();
}

GlGraph::GlGraph(QWidget* parent):QOpenGLWidget(parent)
{
   data_init();
   setFormat(QSurfaceFormat::defaultFormat()); // Двойная буферизация
  glDepthFunc(GL_LEQUAL); // Буфер глубины


  setMouseTracking(true);
}


void GlGraph::setPoints(QList<QPointF> p)
{
 points.clear();
 qDebug()<<p.size();
 points.append(p);
 qDebug()<<points.size();
 update();
  //  points = p;
}

QList<QPointF> GlGraph::getPoints()
{
    return points;//points;
}



void  GlGraph::initializeGL()
{
    qDebug()<<"width = "<<width()<<" height = "<<height();
    win_size.setWidth(width());
    win_size.setHeight(height());
    //qglClearColor(Qt::black); // Черный цвет фона
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины

}

void  GlGraph::resizeGL(int w, int h)
{

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport((GLint)0, (GLint)0, (GLint)w, (GLint)h);

    win_size.setWidth(w);
    win_size.setHeight(h);
}

void  GlGraph::paintGL()
{


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины

    self_axis();

    glMatrixMode(GL_PROJECTION); // устанавливаем матрицу
    glLoadIdentity(); // загружаем матрицу
    glOrtho(0,400,400,0,1,0); // подготавливаем плоскости для матрицы

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //qglColor(Qt::white);
    glColor3f(1,0,0);

    //renderText(10, 10 , 0, QString::fromUtf8("Len %1").arg(points->length()), QFont() );

    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < points.size(); i++) glVertex2d(points.at(i).x(),points.at(i).y());
    glEnd();


    glScalef(win_size.width()*0.001,win_size.height()*0.001,1.0);


        if(under_mouse) self_cursor();


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
    //updateGL();
}

void GlGraph::leaveEvent(QEvent *me)
{
    under_mouse = false;
    update();
}

void GlGraph::mousePressEvent(QMouseEvent *me)
{
  /*  // При нажатии левой кнопки мыши определяем попали мы в гуся или нет?
    if(me->button()==Qt::LeftButton){
        if(me->x()>gdx&&me->x()<gdx+geese_size){
            if(me->y()>gdy&&me->y()<gdy+geese_size){
                point++;
                geese_coord();
            }
        }
    }

    // Если была нажата левая кнопка мыши, получаем начальные координаты выделения
    if(me->button()==Qt::LeftButton){
        singling=true;
        cbx=me->x();
        cby=me->y();
        updateGL();
    } else {
        singling=false;
    }
    updateGL();*/
}

void GlGraph::mouseReleaseEvent(QMouseEvent *me)
{
    // Если отпускаем левую кнопку мыши - удалить выделение
  /*  if(singling==true&&me->button()==Qt::LeftButton){
        singling=false;
    }
    updateGL();*/
}

void GlGraph::self_cursor()
{
    glMatrixMode(GL_PROJECTION); // устанавливаем матрицу
    glLoadIdentity(); // загружаем матрицу
    glOrtho(0,win_size.width(),win_size.height(),0,1,0); // подготавливаем плоскости для матрицы


        glColor3f(1,1,1);// Цвет курсора
        // Координаты курсора
        float x = mouse_pos.x();
        float y = mouse_pos.y();

        glBegin(GL_LINES);
        glVertex2d(x-10,y);
        glVertex2d(x+10,y);
        glVertex2d(x,y-10);
        glVertex2d(x,y+10);
        glEnd();
}

void GlGraph::self_axis()
{
    glMatrixMode(GL_PROJECTION); // устанавливаем матрицу
    glLoadIdentity(); // загружаем матрицу
    glOrtho(0,win_size.width(),win_size.height(),0,1,0); // подготавливаем плоскости для матрицы

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 16));

        glColor3f(0.1,0.1,0.1);

        int hline_count = 10;
        float h_step = win_size.width()/hline_count;

        int wline_count = 10;
        float w_step = win_size.height()/wline_count;

        glBegin(GL_LINES);
        for(int i = 0; i <hline_count; i++){
        glVertex2d(i*h_step,1);
        glVertex2d(i*h_step,win_size.height()-1);
        }

        for(int i = 0; i <wline_count; i++){
        glVertex2d(1,i*w_step);
        glVertex2d(win_size.width()-1,i*w_step);

        }
        glEnd();

        for(int i = 0; i <hline_count; i++)
            painter.drawText(i*h_step, 1, 40, 40, Qt::AlignCenter, QString::fromUtf8("%1").arg(i));
        for(int i = 0; i <wline_count; i++)
            painter.drawText(1, i*w_step, 40, 40, Qt::AlignCenter, QString::fromUtf8("%1").arg(i));
        painter.end();

}


