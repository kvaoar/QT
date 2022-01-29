

#ifndef SPARSER_H
#define SPARSER_H

#include <QObject>
#include <QHostInfo>
#include<QtMath>
#include<QTypeInfo>
#include <QMetaType>

struct SParser: public QObject
{
Q_OBJECT
public:
   // SParser(QObject *parent = nullptr);
    SParser( QString n = "Base", QMap<QString, QPair<qreal,qreal>> c = {});
    virtual ~SParser();

    QString name();
    QStringList channels();
    QPair<qreal,qreal> range(QString ch);
    QMap<QString,QPair<qreal,qreal>> ranges();
    virtual QMap<QString,qreal> parser (QByteArray data);
    QString _name;
    QMap<QString, QPair<qreal,qreal>> _channels;
private:
    friend QDebug operator<<(QDebug dbg, const SParser &x);
};


Q_DECLARE_METATYPE(SParser*)

#endif // SPARSER_H
