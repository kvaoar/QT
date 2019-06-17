#include "gpiomodel.h"

GPIOmodel::GPIOmodel(QObject *parent)
  : QAbstractTableModel(parent)
{

}

void GPIOmodel::add(QString s)
{
  GPIO_cfg cfg;
  QStringList l = s.split(" ",QString::SkipEmptyParts);
  if(l.count() == 5){
     QString port = l.at(0).left(2);
     QString pin = l.at(0).right(l.at(0).length()-2);
     QString dir = l.at(2);
     QString pull = l.at(4);
     cfg.port = map_port.key(port);
     cfg.pin = pin.toInt();
     cfg.dir = map_dir.key(dir);
     cfg.pull = map_pull.key(pull);
     add(cfg);
    }
}

void GPIOmodel::add(GPIOmodel::GPIO_cfg cfg)
{
  gpio_map.insert(QStringLiteral("%1%2").arg(map_port.value(cfg.port)).arg(cfg.pin),cfg);
  QModelIndex topLeft = index(0, 0);
  QModelIndex bottomRight = index(gpio_map.count(),3);
  emit layoutChanged();
  emit dataChanged(topLeft,bottomRight);
}

void GPIOmodel::lvl_changed(QString pin_fullname, bool state)
{
   GPIO_cfg cfg = gpio_map.value(pin_fullname);
   cfg.state = state;
   gpio_map.insert(pin_fullname,cfg);
   QModelIndex topLeft = index(0, 0);
   QModelIndex bottomRight = index(gpio_map.count(),3);
   emit layoutChanged();
   emit dataChanged(topLeft,bottomRight);
}

void GPIOmodel::lvl_changed(GPIOmodel::PORT p, int pin, bool state)
{
  QString key = QStringLiteral("%1%2").arg(map_port.value(p)).arg(pin);
 lvl_changed(key,state);
}

GPIOmodel::GPIO_cfg GPIOmodel::at(int index)
{
  if(index< gpio_map.count()){
      return gpio_map.values().at(index);
    }
  return GPIO_cfg();
}

QList<GPIOmodel::GPIO_cfg> GPIOmodel::at(QList<int> indexes)
{
  QList<GPIOmodel::GPIO_cfg> records;
  foreach(int index, indexes) records.append( gpio_map.values().at(index));
  return records;
}

QStringList GPIOmodel::key_at(QList<int> indexes)
{
  QStringList keys;
  foreach(int index, indexes) keys.append( gpio_map.keys().at(index));
  return keys;
}

QStringList GPIOmodel::key_at(GPIOmodel::PORT port, GPIOmodel::PIN_Direction dir, GPIOmodel::PIN_Pull pull)
{
  QStringList keys;
  QList<GPIO_cfg> all_pin = gpio_map.values();
  foreach(GPIO_cfg cfg, all_pin){
    if(
       ((dir == SKIP_DIR)||(cfg.dir == dir))&&
       ((port == SKIP_PORT)||(cfg.port == port))&&
       ((pull == SKIP_PULL)||(cfg.pull == pull))
       ){
        keys.append(QStringLiteral("%1%2").arg(map_port.value(cfg.port)).arg(cfg.pin));
      }
    //keys.append(gpio_map.key(cfg));
    }
  return keys;
}



void GPIOmodel::remove(QStringList keys)
{
foreach(QString key, keys) gpio_map.remove(key);

QModelIndex topLeft = index(0, 0);
QModelIndex bottomRight = index(gpio_map.count(),3);
emit layoutChanged();
emit dataChanged(topLeft,bottomRight);
}

QVariant GPIOmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
          switch (section) {
          case 0:
              return QString("Pin");
          case 1:
              return QString("Mode");
          case 2:
              return QString("Pull");
          case 3:
              return QString("State");
          }
      }
      return QVariant();
}

int GPIOmodel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return gpio_map.count();
  return gpio_map.count();
  // FIXME: Implement me!
}

int GPIOmodel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 4;
return 4;
  // FIXME: Implement me!
}

QVariant GPIOmodel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();
if (role == Qt::DisplayRole){

  GPIO_cfg cfg = gpio_map.values().at(index.row());
  switch (index.column()) {
    case 0:
        return QStringLiteral("%1%2").arg(map_port.value(cfg.port)).arg(cfg.pin);
    case 1:
        return map_dir.value(cfg.dir);
    case 2:
        return map_pull.value(cfg.pull);
    case 3:
        return cfg.state?"ON":"off";
    }
  }
  // FIXME: Implement me!
  return QVariant();
}
