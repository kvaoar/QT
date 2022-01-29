#ifndef METACURSORTABLE_H
#define METACURSORTABLE_H
#include <QAbstractTableModel>
#include<QTableView>
#include<QColor>
#include<QtAlgorithms>
#include<QAction>
#include<QDateTime>
#include<QMenu>
#include"Cursors/cursor.h"
#include"Units/torr.h"
#include<QTimeEdit>
#include"Cursors/metacursor.h"

class MetaCursorTable: public QAbstractTableModel{
Q_OBJECT
public:

 MetaCursorTable(QObject *parent)
 : QAbstractTableModel(parent)

{
//combobox = new ComboBoxItemDelegate(this);


}
QList<int> ckey_storage;
 QVector<MetaCursor> storage;
void SetCursorMap(QMap<QString,QColor>* c){
col = c;
}

int getID(){return i++;}

    void SetTableControls(QTableView *t)
    {
        table = t;
        table->setModel(this);
        sel = new QItemSelectionModel(this);
       // time_editor = new TimeEditItemDelegate(t);
        //table->setItemDelegateForColumn(CursorTable::SC_TIME,time_editor);
        table->setSelectionModel(sel);
        table->setSelectionMode( QAbstractItemView::SelectionMode::MultiSelection);
        table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows );
        table->setContextMenuPolicy(Qt::ContextMenuPolicy::ActionsContextMenu);
        QAction * actionDel = new QAction("&Del", this);
        QAction * actionGrp = new QAction("&Group", this);
        table->addAction(actionGrp);
        table->addAction(actionDel);


        table->setMouseTracking(true);

        connect(actionDel, SIGNAL(triggered()), this, SLOT(clickDel()));
        connect(actionGrp, SIGNAL(triggered()), this, SLOT(clickGrp()));

        connect(sel,SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,SLOT(onselectionChanged(const QItemSelection &, const QItemSelection &)));
        table->setAutoScroll(false);

    }
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {

        if (role == Qt::DisplayRole){
            if(orientation ==Qt::Orientation::Horizontal)
            {
                 if (section < columnCount())
                return (QString(title.value(section)));
                 else
                 return QVariant();
            }
            else
                return (QString("%1").arg(section));
        }




        return QVariant();

    }
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return storage.count();
    }


    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return 2;
    }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        //qDebug()<<"data"<<index.row()<<index.column();
        MetaCursor l = storage[index.row()];

        if (role == Qt::DisplayRole) {
            QVariant answer;

            switch(index.column()){
            case SC_TIME: {
                QString res;
                /*if(!l.is_abs()){
                int s = std::abs(((l.t)/1000)%60);
                int m = std::abs(((l.t)/(60*1000))%60);
                int h = std::abs((l.t)/(60*60*1000));
                 res.sprintf("- %d:%d:%d",h,m,s);
                } else {
                res = "abs"+QDateTime::fromMSecsSinceEpoch(l.t).toString("hh:mm:ss");
                }
                answer.setValue(res);*/
                }break;
            case SC_NAME: answer = QString("%1-%2").arg(l.count()).arg(l.series());
            break;
            default:
                break;
            }
            return (answer);
        }
        if(role == Qt::BackgroundRole){

           /* QColor c = col->value(l.ser());
            c.setAlpha(100);
            return QVariant(c);*/
        }
        if(role == Qt::EditRole){

            //return  l.t;
        }
        if(role == Qt::WhatsThisRole){

           // return  l.ser();
        }
        if(role == Qt::UserRole){

        }

        return QVariant();
    }
    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        Qt::ItemFlags flags = QAbstractTableModel::flags( index );
        if( index.isValid() ) {
            if( index.column() == SC_TIME ) {
                flags |= Qt::ItemIsEditable;
            }
        }

        return flags;
    }
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override
    {
        if( !index.isValid() || role != Qt::EditRole || storage.count() <= index.row() ) {
            return false;
        }


        qDebug()<<value;
        //QString key = storage->keys().at(index.row());
       // storage->value(key)->color = QColor(value.toString());
        if( index.column() == SC_TIME ) {
           /* storage[index.row()].t = value.toLongLong();
            storage[index.row()].lost();*/
            sel->clear();
        }
        emit dataChanged( index, index );

        return true;
    }



   bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex())override {
        Q_UNUSED(parent)

       if (count < 1 || row < 0 || row > rowCount())
           return false;

       beginInsertRows(QModelIndex(), row, row + count - 1);

       endInsertRows();
       emit cursor_upd();
       return true;
   }

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
    {

        if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
            return false;

        beginRemoveRows(QModelIndex(), row, row+count-1);

        for (int r = 0; r < count; ++r) {
            int del_pos = row+count-1-r;

            //Cursor c = this->data(index(row+count-1-r,0),Qt::UserRole).value<Cursor>();

            storage.removeAt(del_pos);


        }

        endRemoveRows();

        emit cursor_upd();
        return true;
    }


    enum CurColumn{SC_NAME = 0, SC_TIME =1};
public slots:
    void sel_curlist(QVector<Cursor> l){
        ins(MetaCursor(l,getID()));
    }

    void onselectionChanged(const QItemSelection &selected, const QItemSelection &deselected){
        QModelIndexList l = selected.indexes();
        foreach(QModelIndex i, l) storage[i.row()].sel(true);
        QModelIndexList ld = deselected.indexes();
        foreach(QModelIndex i, ld) storage[i.row()].sel(false);
        emit cursor_upd();
    }
    void clickGrp(){

    }

    void clickDel()
    {
        QVector<QModelIndex> list = sel->selectedRows().toVector();
        sel->clear();
qDebug()<<list;
        beginResetModel();
        QList<int> mm;
        while(!list.isEmpty()){
            //Cursor c = this->data(list.last(),Qt::UserRole).value<Cursor>();

            mm.append(list.last().row());
            list.removeLast();
        };

        foreach(int i, mm)
            storage.removeAt(i);
        endResetModel();
        emit cursor_upd();
    }

    /*void rem(MetaCursor c){
        //int row = storage.indexOf(c);
      //  beginRemoveRows(QModelIndex(), row, row);
            //qDebug()<<"void rem(Cursor* c)"<<c;
       //     storage.removeOne(c);
       // endRemoveRows();
    }*/

    void ins(MetaCursor c){
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        //qDebug()<<"void ins(Cursor c)"<<c;
            storage.append(c);
        endInsertRows();
    }

    void upd(QVector<Cursor> cl){
        qDebug()<<"upd(QVector<Cursor> cl)"<<cl.count();
        for (int i = 0; i < storage.count(); i++) {
            storage[i].upd(&cl);
        }
        emit cursor_upd();
    }

    QVector<MetaCursor>* getCur()
    {
        return &storage;
    }



signals:
    void cursor_upd();

private:

 //   TimeEditItemDelegate* time_editor = nullptr;

        //ComboBoxItemDelegate* combobox = nullptr;
    QMap<int,QString> title = {{SC_NAME,"L"},{SC_TIME,"R"}};
    QMap<QString, QColor>* col = nullptr;
     QMenu * menu;
     QTableView* table;
     QItemSelectionModel * sel;
     int i = 0;
};

Q_DECLARE_METATYPE(MetaCursorTable*)
#endif // METACURTABLE_H
