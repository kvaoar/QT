#ifndef FTSTREAM_H
#define FTSTREAM_H

#include <QObject>

class FTStream : public QObject
{
    Q_OBJECT
public:
    explicit FTStream(QObject *parent = nullptr);

signals:

public slots:
};

#endif // FTSTREAM_H