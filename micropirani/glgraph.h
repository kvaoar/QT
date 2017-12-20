#ifndef GLGRAPH_H
#define GLGRAPH_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QObject>
#include <QPainter>

class GlGraph : public QOpenGLWidget
{
    Q_OBJECT

public:
    GlGraph();
    GlGraph(QWidget *parent);

    void setPoints(QList<QPointF> p);
    QList<QPointF> getPoints();



protected:
    QSize win_size; // Размеры окна
    QPoint mouse_pos;
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();


    void mouseReleaseEvent(QMouseEvent *me);
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
    void leaveEvent(QEvent *me);
    void keyPressEvent(QKeyEvent *ke);
private:
    void data_init();
    QList<QPointF> points;

    void self_cursor();
    void self_axis();
    bool under_mouse = false;
};

#endif // GLGRAPH_H
