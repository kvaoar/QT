#ifndef CURSORTABLE_H
#define CURSORTABLE_H
#include <QAbstractTableModel>

#include<QTableView>
#include<QColor>
#include<QtAlgorithms>
#include<QAction>
#include<QDateTime>
#include<QMenu>

#include"Units/torr.h"
#include<QTimeEdit>
#include<QShortcut>

#include"timeedititemdelegate.h"

#include"Cursors/cursor.h"
#include"Cursors/metacursor.h"

class CursorTable: public QAbstractTableModel{
Q_OBJECT
public:

 CursorTable(QObject *parent);
 void SetTableControls(QTableView *t);
 QVector<Cursor> *getCur();

 int count();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    /*bool removeRow(int row, const QModelIndex &parent) {
        if(  storage->count() <= row ) {
            return false;
        }

        Cursor* c = storage->values().at(row);
        storage->remove(storage->key(c),c);

        return true;
    }

*/
   bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex())override;
   bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    enum CurColumn{SC_NAME = 0, SC_TIME =1};
public slots:
    void onselectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void clickGrp();
    void clickDesel();
    void clickDel();
    void rem(Cursor c);
    void ins(Cursor c);
    void upd(Cursor c);
    void upd(QVector<Cursor> cl);
    void invalidate_series(QString ser);
    void set_col(QString ser, QString color);
signals:
    void cursor_upd();
    void cursors_grp(QVector<Cursor> l);
private:
    QMap<QString, QColor> ser_color;
    QVector<Cursor> storage;
    TimeEditItemDelegate* time_editor = nullptr;
        //ComboBoxItemDelegate* combobox = nullptr;
    QMap<int,QString> title = {{SC_NAME,"L"},{SC_TIME,"T"}};
     QMenu * menu;
     QList<QShortcut*> shortcuts;
     QTableView* table;
     QItemSelectionModel * sel;
};
#endif // CURTABLE_H
