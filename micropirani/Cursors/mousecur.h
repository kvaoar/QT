#ifndef CURSORM_H
#define CURSORM_H
#include<QObject>
#include<QtMath>
#include<QDebug>
#include<QPointF>
#include<QSize>
#include"cursor.h"
#include"Series/scaler.h"

class MouseCur: public QObject
{
    Q_OBJECT
    //Q_DISABLE_COPY(CursorM)


public:
    explicit MouseCur(QObject *parent = nullptr);
    void setScaler(Scaler* scal);


    bool found_unic() const;
    bool found()const;

    bool grab_mode() const;
    void drop_grabbed();

public slots:
    void move_mouse(QPointF mouse);
    void add_cursor();
    void grab_cursor();
    void del_cursor();
    void abs_cursor(bool _is_abs);
    void curselect(Cursor c);
    void serselect(const Series* ser, qint64 t);
    void serlost(const Series* ser);
    void lost();

signals:
    void refresh_p(QMap<QString,FoundP> title);
    void move_grabbed(QPointF mouse, Cursor c);

    void newcursor(QString ser, FoundP p, bool is_abs);
    void delcursor(Cursor c);
    void abs_changed(bool);
private:
    QMap<QString,FoundP> found_series;
    QMap<QString, Cursor> found_cur;
    Cursor grabbed_cur;
    qint64 t;

    bool is_found = false;

    bool is_abs = false;
    bool _tmp_abs = false;

    bool grabbed = false;

};
Q_DECLARE_METATYPE(MouseCur*)
#endif // CURSORM_H
