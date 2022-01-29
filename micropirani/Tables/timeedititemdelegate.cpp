#include"timeedititemdelegate.h"

TimeEditItemDelegate::TimeEditItemDelegate(QObject *parent)    : QStyledItemDelegate(parent){}

QWidget *TimeEditItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QTimeEdit *cb = new QTimeEdit(parent);
    cb->setMinimumTime(QTime(0,0,1));
    cb->setMaximumTime(QTime(1,0,0));
    cb->setDisplayFormat("hh:mm:ss");



    /* cb->setDisplayFormat("hh:mm:ss");
        cb->setSelectedSection(QTimeEdit::SecondSection);
        cb->setCurrentSectionIndex(0);*/

    qDebug() <<cb->currentSection()<<cb->currentSectionIndex();
    return cb;
}

void TimeEditItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{


    QTimeEdit *cb = qobject_cast<QTimeEdit *>(editor);
    Q_ASSERT(cb);
    qDebug() <<cb->currentSection()<<cb->currentSectionIndex();

    // get the index of the text in the combobox that matches the current value of the item
    const qint64 t = index.data(Qt::EditRole).toLongLong();
    if(t>0)
        cb->setTime(QDateTime::fromMSecsSinceEpoch(t).time());
    else
        cb->setTime(QDateTime::fromMSecsSinceEpoch(-t).toUTC().time());

    qDebug() <<cb->currentSection()<<cb->currentSectionIndex();



}

void TimeEditItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTimeEdit *cb = qobject_cast<QTimeEdit *>(editor);
    Q_ASSERT(cb);

    qDebug() <<cb->currentSection()<<cb->currentSectionIndex();
    const qint64 t = index.data(Qt::EditRole).toLongLong();
    if(t>0)
        model->setData(index, QDateTime(QDateTime::fromMSecsSinceEpoch(t).date(), cb->time()).toMSecsSinceEpoch(), Qt::EditRole);
    else
        model->setData(index, -1000*(((cb->time().hour()*60)+cb->time().minute())*60+cb->time().second()), Qt::EditRole);

}

void TimeEditItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
