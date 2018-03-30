#include "cursorstorage.h"

#include<QDebug>
#include<QDateTime>

CursorStorage::CursorStorage(QObject *parent, Axis *ax_x, QSize *window) : QAbstractListModel(parent)
{
    ax = ax_x;
    mouse = new Cursor();
    win = window;
}

void CursorStorage::add(QPointF pos, bool c_abs)
{
    if(!exist(pos)){
        Cursor* c = new Cursor(pos,win,c_abs);
    if(c_abs){
            beginInsertRows(QModelIndex(),cur.length(),cur.length());
            cur.append(c);
            c->time_axis_pos = c->axe_pos(ax);
        }else
            dcur.append(c);

    std::sort(cur.begin(),cur.end(),
          [](const Cursor* a, const Cursor* b) -> bool { return a->fpos > b->fpos; });

    scur.all_found = false;
    endInsertRows();
    }
}

void CursorStorage::del(QPointF pos)
{
    for(int i = 0; i < cur.size(); i++)
        if(abs(cur.at(i)->win_pos() - pos.x())<= 3) {
            beginRemoveRows(QModelIndex(),i,i);
            Cursor* c = cur.at(i);
            delete c;
            cur.removeAt(i);
            endRemoveRows();
        }
}

void CursorStorage::start_scan(qint64 last_time, qint64 mouse_t)
{
    mouse->time_axis_pos = mouse_t;
    mouse->is_found = false;
    scur.all_found = false;
    _last_time = last_time;
    if(cur.size() > 0){
        scur.index = 0;
        scur.key = get_time(scur.index);
    }
    for(int i = 0; i < cur.size(); i++) cur.at(i)->is_found = false;
}

Cursor* CursorStorage::process_scan(TimeValPoint p)
{
    Cursor* res = NULL;
    if(!mouse->is_found){
        if(mouse->time_axis_pos >= (p.t - _last_time)){
            mouse->p = p;
            mouse->is_found = true;
        }
    }

    if(cur.size() > 0)
    if(!scur.all_found){
        if(scur.key >= (p.t - _last_time) ) {
            cur.at(scur.index)->p = p;
            cur.at(scur.index)->is_found = true;
            res = cur.at(scur.index);
            if(scur.index < cur.size()-1){
                scur.index++;
                scur.key = get_time(scur.index);
            } else {
                scur.all_found = true;
            }
        }
    }
    return res;
}

void CursorStorage::stop_scan()
{
   // for(int i = 0; i < cur.size(); i++) if(!cur.at(i)->is_abs)if(!cur.at(i)->is_found) cur.removeAt(i);
   //layoutChanged();
if(cur.size()>0) emit QAbstractListModel::dataChanged(index(0),index(cur.size()-1));
}

bool CursorStorage::exist(QPointF pos)
{
   for(int i = 0; i < cur.size(); i++)
       if(cur.at(i)->win_pos() == pos.x()) return true;
   return false;
}

qint64 CursorStorage::win_to_vscr(int x)
{
 //   qDebug()<<win->width();
    float f3 = ((1.0*x)/(1.0*win->width()))*(ax->max()-ax->min()) + ax->min();
//qDebug()<<"win to vscr"<<f3;
    return f3;
}


/**
 * @brief CursorStorage::get_time
 * get time on time axis  from wim pos for abs cursors or from writed time point
 * for cursors fixed on one data stream position
 * @param c
 * @return
 */
qint64 CursorStorage::get_time(int c)
{
    if(cur.at(c)->is_abs){
        return cur.at(c)->axe_pos(ax);
    }
    else
        return cur.at(c)->time_axis_pos;
}

QHash<int, QByteArray> CursorStorage::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TimeRole] = "time";
    roles[ValRole] = "value";
    roles[UnitRole] = "unit";
    roles[FoundRole] = "is_found";
    roles[SelectRole] = "is_selected";
    return roles;
}

QVariant CursorStorage::data(const QModelIndex &index, int role) const
{
    if(index.row()<0 || index.row() >= cur.count()) return QVariant();
//qDebug()<<"row="<<index.row()<<"role="<<roleNames()[role];
    const Cursor* ccur = cur.at(index.row());
    QString val = Torr::toString(ccur->p.f);
    if(role == TimeRole)
        return QVariant::fromValue(QDateTime::fromMSecsSinceEpoch(ccur->time_axis_pos).toString("hh:mm:ss"));
    else if (role == ValRole)
        return QVariant::fromValue(val.split(" ").at(0));
    else if (role == UnitRole)
        return QVariant::fromValue(val.split(" ").at(1));
    else if (role == FoundRole)
        return QVariant::fromValue(ccur->is_found);
    else if (role == SelectRole)
        return QVariant::fromValue(ccur->is_sel);

    return QVariant();
}

bool CursorStorage::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if (!index.isValid()) {
        return false;
    }

    if (role == SelectRole) {
            cur.at(index.row())->is_sel = value.toBool();
            emit  QAbstractListModel::dataChanged(index, index);
            return true;
    }
    return false;
}
Qt::ItemFlags CursorStorage::flags(const QModelIndex &index) const
{
    if(index.column() == TimeRole)
        return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
    else if (index.column() == ValRole)
        return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
    else if (index.column() == UnitRole)
        return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
    else if (index.column() == FoundRole)
        return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
    else if (index.column() == SelectRole)
        return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemNeverHasChildren;
}
