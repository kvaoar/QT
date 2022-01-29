#include "sparser.h"
/*
SParser::SParser(QObject *parent) : QObject(parent)
{

}*/

SParser::SParser(QString n , QMap<QString, QPair<qreal,qreal>> c )
    :_name(n),_channels(c)
{}

SParser::~SParser()
{
qDebug()<<"SParser::~SParser()";
}

QString SParser::name()
{

    return _name;
}

QStringList SParser::channels()
{
    return _channels.keys();
}

QPair<qreal, qreal> SParser::range(QString ch)
{
    return _channels.value(ch);
}

QMap<QString, QPair<qreal, qreal> > SParser::ranges()
{
    return _channels;
}

QMap<QString, qreal> SParser::parser(QByteArray data)
{

            return {{_name+" frombase"+data,0}};

}

QDebug operator<<(QDebug dbg, const SParser &x){
    dbg.nospace()<<"SParser( name="<<x._name
                <<" chan="<<x._channels<<")";


    return dbg.maybeSpace();}
