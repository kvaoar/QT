#ifndef METACURSOR_H
#define METACURSOR_H

#include "cursor.h"
#include"diff1.h"
#include"diff2.h"
#include"linexpdecoder.h"


class MetaCursor{
public:
    MetaCursor();
    MetaCursor(const MetaCursor &cop);
    MetaCursor &operator=(const MetaCursor &in);
    MetaCursor(QVector<Cursor> c, int _id);
    int getID()const;
    bool chk_cursors(XInterval x);
    bool upd_cursor(Cursor c);
    bool upd_cursors(QVector<Cursor> *cl);
    void upd(QVector<Cursor> *cl);
    void sel(bool s);
    bool isEmpty()const;
    QString series()const;
    const QVector<QMap<qint64,qreal>>* plot() const;
    const QVector<Cursor > curlist() const;
    bool sel();
    bool killme()const;
    int count() const;
    friend bool operator==(const MetaCursor& lhs, const MetaCursor& rhs);

private:
QVector<Cursor> cur_list;
QString ser;
QVector<QMap<qint64, qreal>> add_plot;
bool is_sel = false;
bool kill_me = false;
int id;
};

Q_DECLARE_METATYPE(MetaCursor)

#endif // METACURSOR_H
