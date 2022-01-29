#ifndef SERIESTABLE_H
#define SERIESTABLE_H
#include <QAbstractTableModel>
#include<QTableView>
#include<QColor>
#include"Series/series.h"
#include"combocoloritemdelegate.h"

class SeriesTable: public QAbstractTableModel{
Q_OBJECT
public:

 SeriesTable(QObject *parent);

void SetSeriesMap(QMap<QString,Series*>* ser);

    void SetTableControls(QTableView *table);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    enum SeriesColumn{SC_NAME = 1, SC_SCALE =2, SC_EN = 0};
public slots:
    void external_update(bool process);
signals:
    void color_modified(QString ser);
    void enable_modified(QString ser);
private:
    QMap<QString,Series*>* series = nullptr;
    ComboColorItemDelegate* combobox = nullptr;
    QMap<int,QString> title = {{SC_NAME,"name"},{SC_SCALE,"range"},{SC_EN,"en"}};
};

#endif // SERIESTABLE_H

