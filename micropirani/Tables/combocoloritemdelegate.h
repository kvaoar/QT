#ifndef COMBOCOLORITEMDELEGATE_H
#define COMBOCOLORITEMDELEGATE_H


#include <QStyledItemDelegate>
#include <QComboBox>

#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QPainter>

class ComboColorItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboColorItemDelegate(QObject *parent = nullptr);


    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget* editor,
        const QStyleOptionViewItem& option, const QModelIndex& index) const override;

   /* void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {

     // QStyleOptionViewItem myOption = option;
      QString text = QColor::colorNames().at(index.row());

        painter->setPen(QColor(text));
        painter->setBrush(QColor(text));

        painter->drawRect(option.rect);
        painter->setPen(QColor("black"));
        painter->drawText(option.rect.left(),option.rect.bottom(),text);
      //myOption.text = " ";

      //QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
    }*/
};

#endif // COMBOCOLORITEMDELEGATE_H
