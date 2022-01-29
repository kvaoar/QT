#include"metacursor.h"

MetaCursor::MetaCursor(){}

MetaCursor::MetaCursor(const MetaCursor &cop):
    cur_list(cop.cur_list),
    ser(cop.ser),
    add_plot(cop.add_plot),
    is_sel(cop.is_sel),
    kill_me(cop.kill_me),
    id(cop.id)
{}

MetaCursor &MetaCursor::operator=(const MetaCursor &in){
    cur_list = in.cur_list;
    add_plot = in.add_plot;
    is_sel =in.is_sel;
    kill_me =in.kill_me;
    ser =in.ser;
    id =in.id;
    return *this;
}

MetaCursor::MetaCursor(QVector<Cursor > c, int _id){
    id = _id;
    if(!c.isEmpty()){
        ser = c.first().ser();
        for(Cursor cur :c){
            if(cur.ser() == ser ) cur_list.push_back(std::move(cur));
        }
    }
}

int MetaCursor::getID() const {return id;}

bool MetaCursor::chk_cursors(XInterval x){
    foreach(Cursor c, cur_list) if(!x._contain(c.pt)) return false;
    return true;
}

bool MetaCursor::upd_cursor(Cursor c)
{
     int idx = cur_list.indexOf(c);
     if(idx == -1) return false;
     cur_list[idx]=c;
     return true;
}



bool MetaCursor::upd_cursors(QVector<Cursor>* cl){
    for (int i = 0; i < cur_list.count();i++) {
        int idx = cl->indexOf(cur_list[i]);
        if(idx == -1) return false;
        cur_list[i] = cl->at(idx);
    }
    return true;
}

void MetaCursor::upd(QVector<Cursor>* cl){
    add_plot.clear();
    bool up = upd_cursors(cl);
    if(up == false){kill_me = true; return;}
    //if(chk_cursors(x))
    {

        std::sort(cur_list.begin(),cur_list.end(),
                  [](const Cursor& a, const Cursor& b){return a.pt < b.pt;});

        if(cur_list.count()==2){
            diff1 k {cur_list[0],cur_list[1]};
            k.replot(add_plot);
        }
        if(cur_list.count() ==3){
            diff2 kk{{cur_list[0],cur_list[1]},{cur_list[1],cur_list[2]}};
            kk.replot(add_plot);
        }

        if(cur_list.count() == 4){
            diff2 kk1{{cur_list[0],cur_list[1]},{cur_list[1],cur_list[2]}};
            diff2 kk2{{cur_list[1],cur_list[2]},{cur_list[2],cur_list[3]}};
            linexpdecoder le(kk1,kk2);
            le.replot(add_plot);
        }
    } /*else {
        //qDebug()<<"x interval over";
        add_plot.clear();
    }*/
}

void MetaCursor::sel(bool s){is_sel = s;}

bool MetaCursor::isEmpty() const{return !(cur_list.count()>0);}

QString MetaCursor::series() const{return ser;}

const QVector<QMap<qint64, qreal> > *MetaCursor::plot() const {
    return &add_plot;
}

const QVector<Cursor > MetaCursor::curlist() const{return cur_list;}

bool MetaCursor::sel(){return is_sel;}

bool MetaCursor::killme()const{return  kill_me;}

int MetaCursor::count() const{return  cur_list.count();}

bool operator==(const MetaCursor &lhs, const MetaCursor &rhs)
{
    return lhs.id == rhs.id;
}
