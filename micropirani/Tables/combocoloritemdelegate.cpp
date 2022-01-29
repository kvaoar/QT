#include "combocoloritemdelegate.h"


ComboColorItemDelegate::ComboColorItemDelegate(QObject *parent)    : QStyledItemDelegate(parent){}

QWidget *ComboColorItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox *cb = new QComboBox(parent);
    QStringList colors = QColor::colorNames();
    cb->addItems(colors);
    foreach(QString col, colors)
        cb->setItemData(colors.indexOf(col),QColor(col),Qt::BackgroundRole);
    return cb;
}

void ComboColorItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    // get the index of the text in the combobox that matches the current value of the item
    const QString currentText = index.data(Qt::EditRole).toString();
    //qDebug()<<currentText;
    const int cbIndex = cb->findText(currentText);
    // if it is valid, adjust the combobox
    if (cbIndex >= 0)
        cb->setCurrentIndex(cbIndex);
}

void ComboColorItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    model->setData(index, cb->currentText(), Qt::EditRole);

}

void ComboColorItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
