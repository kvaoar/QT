#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include"timevalpoint.h"
#include "torr.h"

#define PACK_SIZ (17)
#define FLOAT_POS (7)
#define FLOAT_LEN (7)

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = nullptr);

signals:
    void dataout(TimeValPoint p);
    void lastval(QString val);
public slots:
    void on_data_rec(QByteArray data, QDateTime time);
    void on_data_upd(QList<TimeValPoint>* p);
    void on_data_ask();
};

#endif // PARSER_H
