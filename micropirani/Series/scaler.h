#ifndef SCALER_H
#define SCALER_H
#include"series.h"
#include"Axis/xaxis.h"
#include"Axis/yaxis.h"
#include"seriesstorage.h"
#include"Cursors/cursor.h"
#include"Cursors/metacursor.h"

#include <QObject>
#include <QComboBox>

typedef QMap<QString,QVector<QPointF>> SerCurves;
typedef QMap<QString,QVector<QPair<QString, QPointF>>> AxisLabels;
typedef QVector<QLineF> AxisLines;
typedef QMap<QString,QColor> SerColors;
typedef QMap<int,QVector<QVector<QPointF>>> MetaCurves;

typedef QMap<qint64, Cursor> CurSeries ;
typedef   QMap<QString,CurSeries >  CurSearch;
typedef QVector<QPair<QString,QPointF>> MouseTitle ;

struct CursorGL{
    bool selected = false;
    QVector<QLineF> lines;
    QPointF text_pos;
    QString text;
    QColor col;
};

typedef QMap<int,CursorGL> gCursors;


class Scaler : public QObject
{
    Q_OBJECT
        Q_PROPERTY(QString status READ status NOTIFY statusChanged)
public:
    explicit Scaler(QObject *parent = nullptr);
    ~Scaler();
    QString status(){return m_status;}

    void setStorage(SeriesStorage* series_stor);

    bool is_log()const;
    void setXComboBox(QComboBox* c);

    QStringList getSeriesList() const;
    bool contains(QString name);

    qreal normalize_x(qint64 val, qint64 lasttime);

    void set_status(QString s);

signals:
void statusChanged(QString newText);

void gl_refresh_curves(SerCurves* curves);
void gl_refresh_colors(SerColors* colors);
void gl_refresh_axis(AxisLines* axis_lines,AxisLabels* axis_labels);
void gl_refresh_cursors(gCursors* gcursors);
void gl_refresh_mcurves(MetaCurves* mcursor);
void gl_refresh_mouse(MouseTitle m_title);
void gl_plot();
void gl_reset();

void mouse_move(QPointF m);

void mouse_addcursor();
void mouse_delcursor();
void mouse_grabcursor();

void mouse_lost();
void mouse_curselect(Cursor c);
void mouse_serselect(const Series* ser, qint64 t);
void mouse_serlost(const Series* ser);
void mouse_setabs(bool abs);

void cur_move(Cursor c);
void cur_found(QVector<Cursor> cl);


void toXComboBoxChanged(int k);

public slots:
void set_lin(bool is_lin);
void zoomT(int k);
void zoomTb(bool inc);
void offsetT(int len);

void ser_onSeriesRefresh(Series* s);
void ser_onSeriesAppend(Series* s);
void ser_onSeriesColorChanged(Series* s);
void ser_onLastTimeUpd(XInterval x);

void cur_onCursorUpdate(QVector<Cursor>* curstor);
void cur_onMetaCursorUpdate(QVector<MetaCursor>* curstor);

void onGlWinsize(QSize win);

void gl_onMouseMove(QPointF m);
void gl_onAddCursor(QPointF m);
void gl_onGrabCursor(QPointF m);
void gl_onDelCursor(QPointF m);
void gl_onInvalidateMouse();
//void gl_onAbsMouse( bool isabs);

void mouse_onUpd(QMap<QString, FoundP> title);
void mouse_onMoveGrabbed(QPointF m, Cursor c);

private slots:
void AskFullRefresh(bool send);
void AskSer(const Series *ser);
void ReportStatus();


private:

void getCursors(gCursors& out, const Series *ser, const CurSearch csc);
void getCurves(QMap<QString, QVector<QPointF> > &out, QSize win_size, const Series *series, XItInterval x);
void getlabels(QSize win_size, QMap<QString, QList<AxeLabel<int> > > &out);
template<typename T, typename P>  void getWinNormLabels(QList<AxeLabel<P>> &out, Axis<T>* a, int k){
    for(auto l = a->labels()->begin(); l != a->labels()->end(); l++)
        out.append({static_cast<P>(a->normalize(l->val)* k),l->text});
}
void getAxis(AxisLines &axis_lines, AxisLabels &axis_labels);


bool log_view = true;

QPoint mouse = {0,0};

XAxis* getX();
YAxis* getY(QString ser = "");
YAxis* getYLog();
YAxis* getYLin();

XAxis* ax = nullptr ;
YAxis* ay_lin = nullptr;
YAxis* ay_log = nullptr;

const YInterval defaultYlogInterval = {1e+3,1e-6};
const YInterval defaultYlinInterval = {760,0};
XInterval x_total = {0,0};

    QMap<QString,const Series*> series;
    QMap<QString,YAxis*> dynrange;

    //QStringList enabled_series;
    QSize win_size;
    QString m_status;
    int offsetT_pos = 0;
    QList<int> zoomT_list = {1, 10,30,1*60,5*60,15*60,30*60,60*60};
    QComboBox* cbx = nullptr;
    QTimer* reporter = nullptr;


    AxisLines axis_lines;
    AxisLabels axis_labels;
    SerCurves curves;
    SerColors curve_color;
    MetaCurves mcurves;
    gCursors gcursors;

    CurSearch rel_cursors;
    CurSearch abs_cursors;
    MouseTitle m_title;

    QMap<QString,QPair<int,int>> decimator_result;


};

#endif // SCALER_H
