#ifndef NETTABLE_H
#define NETTABLE_H
#include <QAbstractTableModel>
#include<QTableView>
#include<Sensors/sensorstate.h>
#include"Parsers/sparser.h"
#include"Parsers/mks925c.h"
#include"Parsers/imr265.h"
#include"combovariantitemdelegate.h"
#include<QDateTime>
#include<QVariant>

class NetTable: public QAbstractTableModel{
Q_OBJECT

public:

 NetTable(QObject *parent);

void SetHostMap(QMap<QString,SensorState>* h);

    void SetTableControls(QTableView *table);
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;


    enum SeriesColumn{SC_NAME = 0, SC_IP =1, SC_ASK,SC_T,/*SC_RANGE,SC_VAL,*/SC_CONN,SC_PARSER, SC_ALL};
public slots:
/*    void external_update(bool process){
if(hosts != nullptr){
        if(process)
            beginResetModel();
        else
            endResetModel();
        }
    }*/

    void ext_upd(QString host, SeriesColumn param );

signals:

private:

    QMap<QString,SensorState>* hosts;
        ComboVariantItemDelegate* combobox = nullptr;
    QMap<int,QString> title = {{SC_NAME,"name"},
                               {SC_IP,"ip"},
                               {SC_ASK,"cmd"},
                               {SC_T,"last t"},
                               /*{SC_RANGE,"Y"},
                               {SC_VAL,"val"},*/
                               {SC_CONN,"online"},
                               {SC_PARSER,"parser"},
                              };

};
#endif // NETWORKTABLE_H
