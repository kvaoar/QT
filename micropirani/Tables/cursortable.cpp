#include"cursortable.h"

CursorTable::CursorTable(QObject *parent)
    : QAbstractTableModel(parent)

{

}

void CursorTable::SetTableControls(QTableView *t)
{
    table = t;
    table->setModel(this);
    sel = new QItemSelectionModel(this);
    time_editor = new TimeEditItemDelegate(t);
    table->setItemDelegateForColumn(CursorTable::SC_TIME,time_editor);
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



    QShortcut* shortcut = new QShortcut(QKeySequence(QKeySequence::Delete), table);
    connect(shortcut, SIGNAL(activated()), this, SLOT(clickDel()));
    shortcuts<<shortcut;
    shortcut = new QShortcut(QKeySequence(QKeySequence::Cancel), table);
    connect(shortcut, SIGNAL(activated()), this, SLOT(clickDesel()));
    shortcuts<<shortcut;

    connect(sel,SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,SLOT(onselectionChanged(const QItemSelection &, const QItemSelection &)));
    table->setAutoScroll(false);

}

QVector<Cursor>* CursorTable::getCur()
{
    return &storage;
}

int CursorTable::count()
{
    return  storage.count();
}


QVariant CursorTable::headerData(int section, Qt::Orientation orientation, int role) const
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

    /*  if (role == Qt::SizeHintRole){
                if(orientation ==Qt::Orientation::Horizontal)
                {
                    QVariant answer;
                    switch(section){
                    case SC_NAME: answer = 80;break;
                    case SC_COLOR: answer = 10;break;
                    case SC_SCALE: answer = 80;break;
                    default:
                        break;
                    }
                    return (answer);
                }
                else
                    return 20;
            }*/


    return QVariant();

}

int CursorTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return storage.count();
}

int CursorTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant CursorTable::data(const QModelIndex &index, int role) const
{
    //qDebug()<<"data"<<index.row()<<index.column();
    Cursor l = storage[index.row()];

    if (role == Qt::DisplayRole) {
        QVariant answer;

        switch(index.column()){
        case SC_TIME: {
            QString res;
            if(!l.is_abs()){
                int s = std::abs(((l.t())/1000)%60);
                int m = std::abs(((l.t())/(60*1000))%60);
                int h = std::abs((l.t())/(60*60*1000));
                res.sprintf("- %d:%d:%d",h,m,s);
            } else {
                res = "abs"+QDateTime::fromMSecsSinceEpoch(l.t()).toString("hh:mm:ss");
            }
            answer.setValue(res);
        }break;
        case SC_NAME: answer = QString("%1-%2").arg(l.ser()).arg(l.is_found());
            break;
        default:
            break;
        }
        return (answer);
    }
    if(role == Qt::BackgroundRole){
        QColor c = Qt::white;
        if(ser_color.contains(l.ser())) c = ser_color.value(l.ser());
        c.setAlpha(100);
        return QVariant(c);
    }
    if(role == Qt::EditRole){

        return  l.t();
    }
    if(role == Qt::WhatsThisRole){

        return  l.ser();
    }
    if(role == Qt::UserRole){

        /*QVariant c;
            c.setValue(l);
            return  c;*/
    }

    return QVariant();
}

Qt::ItemFlags CursorTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    if( index.isValid() ) {
        if( index.column() == SC_TIME ) {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}

bool CursorTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || role != Qt::EditRole || storage.count() <= index.row() ) {
        return false;
    }


    //qDebug()<<value;
    //QString key = storage->keys().at(index.row());
    // storage->value(key)->color = QColor(value.toString());
    if( index.column() == SC_TIME ) {
        storage[index.row()].move(value.toLongLong());
        storage[index.row()].lost();
        sel->clear();
    }
    emit dataChanged( index, index );
    //emit cursor_upd();
    return true;
}

bool CursorTable::insertRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent)

    if (count < 1 || row < 0 || row > rowCount())
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    endInsertRows();
    emit cursor_upd();
    return true;
}

bool CursorTable::removeRows(int row, int count, const QModelIndex &parent)
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

void CursorTable::onselectionChanged(const QItemSelection &selected, const QItemSelection &deselected){
    QModelIndexList l = selected.indexes();
    foreach(QModelIndex i, l) storage[i.row()].sel(true);
    QModelIndexList ld = deselected.indexes();
    foreach(QModelIndex i, ld) storage[i.row()].sel(false);
    emit cursor_upd();
}

void CursorTable::clickGrp(){
    QVector<QModelIndex> list = sel->selectedRows().toVector();
    sel->clear();
    if ((list.count() >1)&&(list.count()<5)) {
        QVector<Cursor> mm;
        while(!list.isEmpty()){
            //Cursor c = this->data(list.last(),Qt::UserRole).value<Cursor>();
            mm.append(storage.at(list.last().row()));
            list.removeLast();
        };





        qDebug()<<mm;
        emit cursors_grp(mm);

    }
}

void CursorTable::clickDesel(){
    sel->clear();
}

void CursorTable::clickDel()
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

void CursorTable::rem(Cursor c){
    int row = storage.indexOf(c);
    beginRemoveRows(QModelIndex(), row, row);
    qDebug()<<"void rem(Cursor* c)"<<c;
    storage.removeOne(c);
    endRemoveRows();


}

void CursorTable::ins(Cursor c){
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    qDebug()<<"void ins(Cursor c)"<<c;
    storage.append(c);
    endInsertRows();
}

void CursorTable::upd(Cursor c){
   int row = storage.indexOf(c);
qDebug()<<"void upd(Cursor c)"<<c.t()<<storage[row].t();
    storage[row] = c;

    //dataChanged(createIndex(row,SC_TIME),createIndex(row,SC_TIME));
}

void CursorTable::upd(QVector<Cursor> cl)
{
    foreach(Cursor c, cl){
        int idx = storage.indexOf(c);
        if(idx>0) storage[idx].found({c.pt,c.pval});
    }
}

void CursorTable::invalidate_series(QString ser)
{

    for(Cursor* c = storage.begin(); c != storage.end(); c++){
        if(c->ser()==ser)c->lost();
    }
    /*QtConcurrent::map(storage,[ser](pCursor  c){
        if(c->ser()==ser)c->lost();
    });*/
}

void CursorTable::set_col(QString ser, QString color)
{
    ser_color[ser] = color;
    qDebug()<<ser_color;
}
