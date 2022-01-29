#ifndef SERIESSTORAGE_H
#define SERIESSTORAGE_H


#include"series.h"
#include"Axis/xaxis.h"
#include"Axis/yaxis.h"
#include"Tables/serisetable.h"
#include<QVariant>
#include<QTableView>
#include"Sensors/myudp.h"

#include<QDebug>
#include<QColor>
#include<QtMath>
#include<QtGlobal>
#include<QFile>
#include<QFileDialog>
#include<QMessageBox>
#include<QDateTime>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonDocument>
#include <QStringListModel>

class SeriesStorage: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    SeriesStorage(QObject *parent = nullptr);
    ~SeriesStorage();

    QString status();

    Series* addSeries(QString name, Series *val);
    void set_series_table(QTableView *ser_table);

    qint64 LastTime()const;
    XInterval XTotal();

    QColor getColor(QString ser);
    SerIt getAt(QString ser, qint64 t)const;
    YInterval getYRange(QString ser, XItInterval x);
     void write(QJsonObject &json) const;
     void read(const QJsonObject &json);
    friend QDataStream &operator<<(QDataStream &, const SeriesStorage &);
    friend QDataStream &operator>>(QDataStream &, SeriesStorage &);

public slots:
    void Add(QString hostname, SensorState *ss, TimeValPoint p);
    void Load();
    void Save();
    void Reset();


private slots:
    void upd_colortable(QString ser);
    void en_modified(QString ser);
    void saveExtChng( const QString& s);

signals:
    void refresh(Series* s);
    void last_time(XInterval x_total);
    void append(Series* s);
    void color_changed(Series* s);
    void lastval(QString name, QString val, QString col);
    void statusChanged(QString newText);

private:
    void set_status(QString s);
    QString m_status;
    SeriesTable* series_model;

    QMap<QString,Series*> series;

    XInterval x_total = {0,0};



    const YInterval defaultYlogInterval = {1e+3,1e-6};
    const YInterval defaultYlinInterval = {760,0};

    QList<QString> known_hosts;
    QString FileFilter = tr("Data (*.json);;Context (*.bin)");
    QFileDialog dialog;
};

Q_DECLARE_METATYPE(SeriesStorage*)

#endif // SERIESSTORAGE_H
