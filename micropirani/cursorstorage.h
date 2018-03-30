#ifndef CURSORSTORAGE_H
#define CURSORSTORAGE_H

#include <QObject>
#include <QMap>
#include<QRectF>
#include<QAbstractListModel>
#include<QByteArray>
#include<QDebug>
#include"cursor.h"
#include"axis.h"
#include"torr.h"

typedef  struct {
int index = 0;
qint64 key = 0;
bool all_found = false;
}SearchState;

class CursorStorage : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CursorStorage(QObject *parent = nullptr, Axis* ax_x = nullptr, QSize* window = nullptr);
  //  void add(QPointF pos);
    void add(QPointF pos, bool c_abs);
    void del(QPointF pos);

    void start_scan(qint64 last_time, qint64 mouse_t);
    Cursor* process_scan(TimeValPoint p);
    void stop_scan();
    bool exist(QPointF pos);
    qint64 win_to_vscr(int x);

    qint64 get_time(int c);
/*
    Q_PROPERTY(bool is_selected READ is_selected WRITE setIs_selected)
    Q_INVOKABLE bool is_selected(){return _is_selested;}
    Q_INVOKABLE void setIs_selected(bool wsel){_is_selested = wsel;}
*/
    QList<Cursor*> cur;
    QList<Cursor*> dcur;
    Cursor* mouse;
signals:

    void dataChanged(QModelIndex, QModelIndex);
public slots:

protected:

    Axis* ax;
    qint64 _last_time = 0;
    QSize* win = NULL;

    SearchState scur;
QHash<int, QByteArray> roleNames() const;

public:

    enum CurRoles {
        TimeRole = Qt::UserRole + 1,
        ValRole,
        UnitRole,
        FoundRole,
        SelectRole
    };



    int rowCount(const QModelIndex &parent) const
    {
    Q_UNUSED(parent);
        return cur.count();
    }
    QVariant data(const QModelIndex &index, int role) const;

public:
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
};


#endif // CURSORSTORAGE_H
