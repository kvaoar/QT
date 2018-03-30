#include<QDebug>
#include "parser.h"



Parser::Parser(QObject *parent) : QObject(parent)
{

}
void Parser::on_data_rec(QByteArray data, QDateTime time){
    QString s =  QString::fromStdString(data.data());

        if(s.size() == PACK_SIZ){
            s = s.toLower().mid(FLOAT_POS,FLOAT_LEN);
            bool ok(false);
            float f = s.toDouble(&ok);
            if(ok){
            emit dataout(TimeValPoint{time.toMSecsSinceEpoch(),f});
            emit lastval(Torr::toString(f));
            }else
                qDebug()<<"RECIEVED:"<< s<<"PARSE ERROR";
        }
        else qDebug()<<"RECIEVED:"<<s<<"LEN ERROR";
}
void Parser::on_data_upd(QList<TimeValPoint>* p){
    qDebug()<<"RECIEVED:"<<p->size()<< "last p "<<p->last().f<<" "<<p->last().t;
}
void Parser::on_data_ask(){
    // qDebug()<<"ASK";
}
