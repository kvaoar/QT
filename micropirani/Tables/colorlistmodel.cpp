#include"colorlistmodel.h"

int ColorListModel::rowCount(const QModelIndex&) const {
    return mItems.size();
}



QVariant ColorListModel::data(const QModelIndex& index, int role) const {
    //qDebug()<<index<<role;
    if(!index.isValid()) return QVariant();
  QObject* item = mItems.at(index.row());

  switch (role) {
    case Qt::DisplayRole: return item->property("name"); break;
   // case Qt::BackgroundRole: return item->property("color"); break;
    case (Qt::EditRole): item->setProperty("role",role); return  QVariant::fromValue(item); break;
  default: return QVariant(); break;
  }


  return QVariant::fromValue(item);
}

bool ColorListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    //qDebug()<<"SET DATA";
        QObject* obj = value.value<QObject*>();

    if(obj->property("name").isValid())
       qDebug()<<obj->property("name").toString();

    if(obj->property("color").isValid()) {
        QString color = obj->property("color").toString();
        qDebug()<<color;
    }

    if(obj->property("font").isValid()){
        QFont f = obj->property("font").value<QFont>();
        qDebug()<<f.toString();
    }

    mItems.replace(index.row(),obj);
     emit dataChanged( index, index );
    emit changed(obj);

    return true;
}

Qt::ItemFlags ColorListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags( index );
    if(index.isValid())
    flags |= Qt::ItemIsEditable;

    return flags;
}

void ColorListModel::insert(QObject* item) {
    beginInsertRows(QModelIndex(), 0, 0);
    mItems.push_front(item);
    endInsertRows();
}

void ColorListModel::remove(QObject* item) {
  for (int i = 0; i < mItems.size(); ++i) {
    if (mItems.at(i) == item) {
      beginRemoveRows(QModelIndex(), i, i);
      mItems.remove(i);
      endRemoveRows();
      break;
    }
  }
}

QHash<int, QByteArray> ColorListModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[Qt::UserRole + 1] = "name";
  roles[Qt::UserRole + 2] = "color";
  roles[Qt::UserRole + 3] = "font";
  return roles;
}
