#ifndef CURSORSTORAGE_H
#define CURSORSTORAGE_H

#include <QObject>

class CursorStorage : public QObject
{
    Q_OBJECT
public:
    explicit CursorStorage(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CURSORSTORAGE_H