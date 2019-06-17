#ifndef GPIOMODEL_H
#define GPIOMODEL_H

#include <QAbstractTableModel>

class GPIOmodel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit GPIOmodel(QObject *parent = nullptr);
  typedef enum{PA=0,PB=1,PC=2, SKIP_PORT =3} PORT;
  typedef enum{IN=0,OUT=1,OD=2, SKIP_DIR =3} PIN_Direction;
  typedef enum{UP=0,DOWN=1,HI_Z=2, SKIP_PULL =3} PIN_Pull;
  typedef struct{
    PORT port;
    int pin;
    PIN_Direction dir;
    PIN_Pull pull;
    bool state = false;
  } GPIO_cfg;
  void add(QString s);
  void add(GPIO_cfg cfg);

   void lvl_changed(QString pin_fullname, bool state);
  void lvl_changed(PORT p, int pin, bool state);
  GPIO_cfg at(int index);
  QList<GPIO_cfg> at(QList<int> indexes);
  QList<GPIO_cfg> at(PIN_Direction dir);

  QStringList key_at(QList<int> indexes);
  QStringList key_at(PORT port, PIN_Direction dir, PIN_Pull pull);
  void remove(QStringList keys);
  QMap<PORT,QString> map_port = {{PA,"PA"},{PB,"PB"},{PC,"PC"}};
  QMap<PORT,int> map_port_pinmax = {{PA,15},{PB,11},{PC,15}};
  QMap<PIN_Direction,QString> map_dir = {{IN,"in"},{OUT,"out"},{OD,"open-drain"}};
  QMap<PIN_Pull,QString> map_pull = {{UP,"up"},{DOWN,"down"},{HI_Z,"floating"}};
  // Header:
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  // Basic functionality:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
  QMap<QString,GPIO_cfg> gpio_map;
  //QList<GPIO_cfg> gpio_list;

};

#endif // GPIOMODEL_H
