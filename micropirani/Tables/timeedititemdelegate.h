#ifndef TIMEEDITITEMDELEGATE_H
#define TIMEEDITITEMDELEGATE_H
#include <QStyledItemDelegate>
#include <QDateTimeEdit>

#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>
#include<QDebug>

class TimeEditItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TimeEditItemDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};


#endif // TIMEDELEGATE_H
