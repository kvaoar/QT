#ifndef COMBOVARIANTITEMDELEGATE_H
#define COMBOVARIANTITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>

#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>
#include<QDebug>

class ComboVariantItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QMap<QString,QVariant> val;

    ComboVariantItemDelegate(QObject *parent = nullptr,QMap<QString,QVariant> v = {});
    //~ComboBoxItemDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget* editor,
        const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /*void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
      QStyleOptionViewItem myOption = option;
      QString text = QColor::colorNames().at(index.row());

      myOption.text = " ";

      QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
    }*/
};


#endif // COMBOVARIANTITEMDELEGATE_H
