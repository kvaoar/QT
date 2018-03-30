#ifndef GLGRAPH_H
#define GLGRAPH_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QPointF>
#include<QComboBox>

#include"cursorstorage.h"
#include "timevalpoint.h"
#include"axis.h"


class GlGraph : public QOpenGLWidget
{
    Q_OBJECT

public:
    GlGraph(QWidget *parent);
    ~GlGraph();


public slots:
    void refresh_data(QList<TimeValPoint>* pt);
    void set_ax_y_lin(bool is_lin);
    CursorStorage* get_curlist();

protected:
    QSize win_size; // Размеры окна
    int points_per_chart; //Число последних отображаемых точек

    QPoint mouse_pos;
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mouseReleaseEvent(QMouseEvent *me);
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
    void wheelEvent(QWheelEvent * me);
    void leaveEvent(QEvent *me);
    void keyPressEvent(QKeyEvent *ke);

private:
    void data_init();
    bool data_exist();
    void plot_mouse(QPainter* painter);
    void plot_cursors(QPainter *painter);
    void plot_axis(QPainter *painter);
    //bool search_point(QPointF vscreen_point, TimeValPoint* res);
    QPointF vscreen_to_win(QPointF p);
    QPointF win_to_vscreen(QPointF p);
    Axis* ax_x = NULL;
    Axis* ax_y = NULL;
    QComboBox time_steps;
    const int minute = 60.0*1000;
    CursorStorage* cursor_storage;
    QRectF vscreen; //OpenGL data-sized screen
    QList<TimeValPoint>* points = NULL;

   // QTimer* timer = NULL;
    bool under_mouse = false;

};

#endif // GLGRAPH_H
