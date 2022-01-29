#ifndef CURSORSTORAGE_H
#define CURSORSTORAGE_H

#include"Series/seriesstorage.h"
#include"cursor.h"
//#include"mousecur.h"
#include"Tables/cursortable.h"
#include"Tables/metacursortable.h"
#include"Series/scaler.h"

#include <QObject>
#include<QStack>


class CursorStorage : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CursorStorage)
public:
    explicit CursorStorage(QObject *parent = nullptr);
    void set_tables(QTableView *table,QTableView *mtable);
    void setScaler(Scaler *sc);

signals:
    void refresh_cursors(QVector<Cursor>* c);
    void refresh_mcursors(QVector<MetaCursor>* c);


public slots:
    void ser_onColorChanged(Series* s);
    void ser_onLastTime(XInterval x_total);
    void add(QString ser, FoundP p, bool is_abs);
    void del(Cursor c);
    void move(Cursor c);
    void found(QVector<Cursor> fcurl);
    void reset();
    void on_ext_upd();
    void on_mcext_upd();

private:
    int get_id();
    int counter = 0;
    CursorTable c_table={this};
    MetaCursorTable mc_table={this};

    bool _is_abs = false;
    bool _tmp_abs = false;

    XInterval x_tot;

};

Q_DECLARE_METATYPE(CursorStorage*)

#endif // CURSORSTORAGE_H
