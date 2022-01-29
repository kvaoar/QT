#include "combovariantitemdelegate.h"


ComboVariantItemDelegate::ComboVariantItemDelegate(QObject *parent, QMap<QString, QVariant> v): QStyledItemDelegate(parent)
{val = v;}

QWidget *ComboVariantItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    // Create the combobox and populate it
    qDebug()<<"create editor";
    QComboBox *cb = new QComboBox(parent);

    foreach(QString l, val.keys()){
        cb->addItem(l,val.value(l));
        //cb->setItemData(i++, val.value(l),Qt::EditRole);
    }
    return cb;
}

void ComboVariantItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    // get the index of the text in the combobox that matches the current value of the item
    const QString currentText = index.data(Qt::DisplayRole).toString();
    const int cbIndex = cb->findText(currentText);
    // if it is valid, adjust the combobox
    if (cbIndex >= 0)
        cb->setCurrentIndex(cbIndex);
}

void ComboVariantItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    int i = cb->currentIndex();
    QString name = val.keys().at(i);
    QVariant var;
    var.setValue(val.value(name));
    model->setData(index,var , Qt::EditRole);

}

void ComboVariantItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
