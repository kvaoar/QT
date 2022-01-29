#ifndef COLORLISTMODEL_H
#define COLORLISTMODEL_H
#include <QObject>
#include <QListView>
#include <QAbstractListModel>
#include <QVector>
#include<QDebug>

class ColorListModel : public QAbstractListModel {
  Q_OBJECT
public:
  int rowCount(const QModelIndex&) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:
  void insert(QObject* item);
  void remove(QObject* item);
  signals:
  void changed(QObject* obj);

protected:
  QHash<int, QByteArray> roleNames() const override;

private:
  QVector<QObject*> mItems;
};

#endif // COLORLISTMODEL_H
