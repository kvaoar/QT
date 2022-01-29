#include "cursorstorage.h"
#include<QAbstractItemView>
#include<QTableView>

CursorStorage::CursorStorage(QObject *parent) : QObject(parent)
{
    connect((&c_table),SIGNAL(cursor_upd()),this,SLOT(on_ext_upd()));
    connect(&mc_table,&MetaCursorTable::cursor_upd,this,&CursorStorage::on_mcext_upd);
    connect((&c_table),SIGNAL(cursors_grp(QVector<Cursor>)), (&mc_table),SLOT(sel_curlist(QVector<Cursor>)));
}


void CursorStorage::set_tables(QTableView *table,QTableView *mtable)
{
    c_table.SetTableControls(table);
    mc_table.SetTableControls(mtable);
}

void CursorStorage::setScaler(Scaler *sc)
{
 connect(this, &CursorStorage::refresh_cursors, sc, &Scaler::cur_onCursorUpdate);
 connect(this, &CursorStorage::refresh_mcursors, sc, &Scaler::cur_onMetaCursorUpdate);
 connect(sc, &Scaler::cur_move, this, &CursorStorage::move);
 connect(sc, &Scaler::cur_found, this, &CursorStorage::found);

}

void CursorStorage::ser_onColorChanged(Series *s)
{
    c_table.set_col(s->name,s->color);
}

void CursorStorage::ser_onLastTime(XInterval x_total)
{
    x_tot = x_total;
    //emit refresh_cursors(c_table.get_cur());
}


void CursorStorage::add(QString ser, FoundP p, bool is_abs){
    Cursor c (is_abs?(p.pt):(p.pt-x_tot.max),ser,get_id());
    c.found(p);
    c_table.ins(c);
    emit refresh_cursors(c_table.getCur());
}

void CursorStorage::del(Cursor c)
{
    qDebug()<<"CursorStorage::del(Cursor c)"<<c;
    c_table.rem(c);
    emit refresh_cursors(c_table.getCur());
}

void CursorStorage::move(Cursor c)
{
    c_table.upd(c);
    emit refresh_cursors(c_table.getCur());
}

void CursorStorage::found(QVector<Cursor> fcurl)
{
    qDebug()<<"CursorStorage::found";
    c_table.upd(fcurl);
    mc_table.upd(fcurl);
}



void CursorStorage::reset()
{
     //emit refresh_cursors(&c_table.storage);
}

void CursorStorage::on_ext_upd()
{
    qDebug()<<"CursorStorage::on_ext_upd()";

        emit refresh_cursors(c_table.getCur());

}

void CursorStorage::on_mcext_upd()
{
        emit refresh_mcursors(mc_table.getCur());
}

int CursorStorage::get_id(){
    return counter++;
}





