#include "config.h"
#include<QDebug>
Config::Config(QObject *parent) : QObject(parent)
{
qDebug()<< settings.fileName();
}

Config::Config(QString file, QObject *parent): QObject(parent) , f_name(file)
{
  qDebug()<< settings.fileName();
}

QString Config::getF_name() const
{
  return f_name;
}

void Config::setF_name(const QString &value)
{
  f_name = value;
}
