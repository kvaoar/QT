#include"serisetable.h"
SeriesTable::SeriesTable(QObject *parent)
    : QAbstractTableModel(parent)

{
    combobox = new ComboColorItemDelegate(this);

}



void SeriesTable::SetSeriesMap(QMap<QString, Series *> *ser){
    series = ser;
    if(ser == nullptr)
        qDebug()<<"NULL PTR!";
    else
        qDebug()<<"Series map ok, cnt ="<<series->count();
}

void SeriesTable::SetTableControls(QTableView *table)
{
    table->setModel(this);
    table->setItemDelegateForColumn(SeriesTable::SC_NAME,combobox);
    table->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    table->setColumnWidth(0,20);
    table->setAutoScroll(false);

}

QVariant SeriesTable::headerData(int section, Qt::Orientation orientation, int role) const
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

int SeriesTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(series == nullptr) return 0;
    return series->size();
}

int SeriesTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return title.count();
}

QVariant SeriesTable::data(const QModelIndex &index, int role) const
{
    //qDebug()<<"data"<<index.row()<<index.column();
    if(!index.isValid()) return QVariant();
    QVariant answer;
    QString s_name = series->keys().at(index.row());
    if (role == Qt::DisplayRole) {
        switch(index.column()){
        case SC_NAME: answer = s_name; break;
        case SC_SCALE: answer = QString("%1:%2")
                    .sprintf("%1.0e:%1.0e",
                             series->value(s_name)->y.max,
                             series->value(s_name)->y.min);
            break;
        default:
            break;
        }
        return (answer);
    }

    if(role == Qt::CheckStateRole){
        switch(index.column()){
        case SC_EN: answer = series->value(s_name)->enable?Qt::Checked:Qt::Unchecked ;
            break;
        default:
            break;
        }
        return (answer);

    }

    if(role == Qt::BackgroundRole){
        QColor c = series->value(s_name)->color;
        if(index.column() == SC_NAME)
            c.setAlpha(200);
        else
            c.setAlpha(100);

        return QVariant(c);
    }

    if(role == Qt::EditRole){
        if(index.column() == SC_NAME)
            return QVariant(series->value(s_name)->color);
        else return QVariant();
    }

    return QVariant();
}

Qt::ItemFlags SeriesTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    if( index.isValid() ) {
        if( index.column() == SC_NAME ) {
            flags |= Qt::ItemIsEditable;
        }
        if( index.column() == SC_EN ) {
            flags |= Qt::ItemIsUserCheckable;
        }

    }

    return flags;
}

bool SeriesTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || series->count() <= index.row() ) {
        return false;
    }

    QString key = series->keys().at(index.row());

    if (role == Qt::CheckStateRole)
    {
        if( index.column() == SC_EN ) {
            if(series->value(key)->enable != value.toBool())
                series->value(key)->enable = value.toBool();
            emit enable_modified(key);
            //qDebug()<<key<<*series->value(key);
            emit dataChanged( index, index );
        }
    }

    if (role == Qt::EditRole)
    {
        if(index.column() == SC_NAME){
            series->value(key)->color = value.toString();
            emit color_modified(key);

            emit dataChanged( createIndex(index.row(),0), createIndex(index.row(),columnCount()) );
        }
    }



    return true;
}

void SeriesTable::external_update(bool process){

    if(process)
        beginResetModel();
    else
        endResetModel();
}
