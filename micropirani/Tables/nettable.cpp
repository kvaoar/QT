#include"nettable.h"

NetTable::NetTable(QObject *parent)
    : QAbstractTableModel(parent)

{
    QVariant imr265;imr265.setValue(new IMR265());
    QVariant mks925;mks925.setValue(new MKS925C());


    combobox = new ComboVariantItemDelegate(this,{
                                                {imr265.value<SParser*>()->name(),imr265},
                                                {mks925.value<SParser*>()->name(),mks925}
                                            });
}

void NetTable::SetHostMap(QMap<QString, SensorState> *h){
    hosts = h;
    if(h == nullptr)
        qDebug()<<"NULL PTR!";
    else
        qDebug()<<"Series map ok ="<<*hosts;
}

void NetTable::SetTableControls(QTableView *table)
{
    table->setModel(this);
    table->setItemDelegateForRow(NetTable::SC_PARSER,combobox);


    table->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    table->setAutoScroll(false);
}

QVariant NetTable::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role == Qt::DisplayRole){
        if(orientation ==Qt::Orientation::Vertical)
        {
            if (section < rowCount())
                return (QString(title.value(section)));
            else
                return QVariant();

        }
        else
            return QVariant();
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

int NetTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return title.count();
}

int NetTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(hosts == nullptr) return 0;
    return hosts->count();
}

QVariant NetTable::data(const QModelIndex &index, int role) const
{
    //        qDebug()<<"data"<<index.row()<<index.column();
    if (role == Qt::DisplayRole) {
        QVariant answer;
        QString s_name = hosts->keys().at(index.column());
        SensorState ss = hosts->value(s_name);
        switch(index.row()){
        case SC_NAME   :answer = s_name;
            break;
        case SC_IP     :answer = ss.ip;
            break;
        case SC_ASK    :answer = ss.ask;
            break;
        case SC_T      :answer = QDateTime::fromMSecsSinceEpoch(ss.last_time).toString("hh:mm:ss");
            break;
            /*case SC_RANGE  :answer = ss.series_range. break;
            case SC_VAL    :answer = break;*/
        case SC_CONN   :answer = QString("%1").arg(ss.is_connected);
            break;
        case SC_PARSER :answer = ss.p->name();
            break;
        default:
            break;
        }
        return (answer);
    }

    return QVariant();
}

Qt::ItemFlags NetTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    if( index.isValid() ) {
        if( index.row() == SC_PARSER ) {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}

bool NetTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || role != Qt::EditRole || hosts->count() <= index.column() ) {
        return false;
    }
    qDebug()<<*value.value<SParser*>();
    QString s_name = hosts->keys().at(index.column());
    SensorState* ss = &hosts->find(s_name).value();
    switch (index.row()) {
    case SC_NAME   :
        break;
    case SC_IP     :ss->ip = value.toString();
        break;
    case SC_ASK    :ss->ask = value.toString();
        break;
    case SC_T      :ss->last_time = value.toLongLong();
        break;
        /*case SC_RANGE  :answer = ss.series_range. break;
        case SC_VAL    :answer = break;*/
    case SC_CONN   :ss->is_connected = value.toBool();
        break;
    case SC_PARSER : { delete ss->p; ss->p = value.value<SParser*>();  auto t = ss->p->ranges(); ss->series_ranges.swap(t); ss->series_val.clear(); }
        break;
    default:
        return false;
        break;
    }

    // QString key = series->keys().at(index.row());
    // series->value(key)->color = QColor(value.toString());
    emit dataChanged( index, index );

    return true;
}

void NetTable::ext_upd(QString host, NetTable::SeriesColumn param){
    int col = hosts->keys().indexOf(host);
    int row = param;
    if(param == SC_ALL){
        foreach(int i, title.keys()){
            emit dataChanged( index(i,col), index(i,col) );
        }
    } else
        emit dataChanged( index(row,col), index(row,col) );
}
