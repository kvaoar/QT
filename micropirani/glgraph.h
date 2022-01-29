#ifndef GLGRAPH_H
#define GLGRAPH_H

//#include <QGLWidget>
//#include <QtOpenGL>
#include<QOpenGLWidget>
#include<QOpenGLBuffer>
#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QPointF>
#include<QComboBox>
#include<QTimer>



#include"Series/seriesstorage.h"
#include"Cursors/cursorstorage.h"
#include"Tables/colorlistmodel.h"
#include"Tables/glstyleeditor.h"

#include"Series/scaler.h"





class GlGraph : public QOpenGLWidget
{
    Q_OBJECT
Q_PROPERTY(int active_upd_ms READ getUpdate_lag_ms WRITE setUpdate_lag_ms  DESIGNABLE true)
Q_PROPERTY(bool active_upd  READ getActive_upd_control WRITE setActive_upd_control  DESIGNABLE true)



public:
    GlGraph(QWidget *parent);
    ~GlGraph();
    enum ColorElements{
        BACK,GRID,SCREENBAR,DATABAR,
        AXELABELS, MOUSECUR, CURLABELS,
        SELCURL
    };
    Q_ENUM(ColorElements)


    QColor color(ColorElements e){return QColor(colormap.value(e));}

    int getUpdate_lag_ms() const;
    bool getActive_upd_control() const;
    void setView(QListView* cl);

    void setScaler(Scaler* scaler);



public slots:
    void setUpdate_lag_ms(int value);
    void setActive_upd_control(bool value);
    void refresh_curves(SerCurves *_curves);
    void refresh_axis(AxisLines *_axis_lines, AxisLabels *_axis_labels);
    void refresh_colors(SerColors* col);
    void refresh_mouse(MouseTitle mtit);


    void set_dots(bool dots) {dot_plot = dots; update();} //Action
    void refresh_cursors(gCursors* cl);
    void refresh_mcursors(MetaCurves* l);
    //void refresh_mouse(MouseCur* s);
    void style_changed(QObject* o);
    void reset();
    void gl_plot();
    void tick();
signals:
    void resize_to_scaler(QSize s);
    void offsetT(int d);
    void zoomT(bool d);


    void mouse_move(QPointF m);
    void add_cursor(QPointF m);
    void grab_cursor(QPointF m);
    void del_cursor(QPointF m);
    void invalidate_mouse();
    void abs_cursor(bool);

protected:
    QSize win_size = {0,0}; // Размеры окна
    QPointF mouse_pos = {0,0};
    bool under_mouse = false;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mouseReleaseEvent(QMouseEvent *me);
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
    void wheelEvent(QWheelEvent * me);
    void leaveEvent(QEvent *me);
    void keyPressEvent(QKeyEvent *ke);
    void keyReleaseEvent(QKeyEvent *ke);

private:
    void data_init();
    bool data_exist();
    void plot_graph(QPainter* painter);
    void plot_mouse(QPainter* painter);
    void plot_cursors(QPainter *painter);
    void plot_axis(QPainter *painter);
    void plot_globalscale(QPainter *painter);

    QTimer* upd_control_timer;
    SerCurves curves;
    SerColors curve_color;

    QMap<QString,qint64> curve_lasttime;
    qint64 lasttime;

    QPen paxis;
    QPen pcursors;
    QPen pcurves;

    AxisLines axis_lines;
    AxisLabels axis_labels;
    gCursors cursors;
    MouseTitle m_title;
    MetaCurves mcurves;


    QPointF norm_to_win(QPointF p);
    QPointF win_to_norm(QPoint p);

    bool dot_plot = false;


    QMap<ColorElements,QString> title = {
        {BACK,"Background"},
        {GRID,"Axis lines"},
        {SCREENBAR,"Screen bar"},
        {DATABAR,"Data bar"},
        {AXELABELS,"Axis text"},
        {MOUSECUR,"Cursor cross"},
        {CURLABELS,"Cursor text"},
        {SELCURL,"Sel cursor text"},
    };
    QMap<ColorElements,QString> colormap = {
        {BACK,"black"},
        {GRID,"silver"},
        {SCREENBAR,"darkslateblue"},
        {DATABAR,"darkorchid"},
        {AXELABELS,"azure"},
        {MOUSECUR,"gold"},
        //{CURLABELS,"goldenrod"}
    };

    QMap<ColorElements,QFont> fontmap = {
        //{BACK,"black"},
       // {GRID,"silver"},
       // {SCREENBAR,"darkslateblue"},
       // {DATABAR,"darkorchid"},
        {AXELABELS,{"Arial", 12, QFont::Bold}},
       // {MOUSECUR,"gold"},
        {CURLABELS,{"Arial", 10, QFont::Bold}},
        {SELCURL,{"Arial", 14, QFont::Bold}}
    };

    ColorListModel* cl_model;
    StyleEditorDelegate* style_delegate;
    Qt::KeyboardModifiers key_mod = Qt::NoModifier;

};
//Q_DECLARE_METATYPE(GlGraph::ColorElements)

Q_DECLARE_METATYPE(GlGraph*)

#endif // GLGRAPH_H
