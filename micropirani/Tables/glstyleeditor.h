#ifndef GLSTYLEEDITOR_H
#define GLSTYLEEDITOR_H
#include<QDateTime>
#include <QWidget>
#include<QComboBox>
#include<QDebug>
#include<QStyledItemDelegate>
#include<QPainter>
namespace Ui {
class glStyleEditor;
}



class glStyleEditor : public QWidget
{
    Q_OBJECT

public:
    explicit glStyleEditor(QWidget *parent = nullptr);
    ~glStyleEditor();

    void getQObject(QObject* obj);
    QObject* setQObject();

private:
    void fillComboBox(QComboBox* cb);
    QStringList colors;
    QObject* editing_obj = nullptr;
    Ui::glStyleEditor *ui;
};

class StyleEditorDelegate : public QStyledItemDelegate{
    Q_OBJECT
    Q_DISABLE_COPY(StyleEditorDelegate)
public:
    explicit StyleEditorDelegate(QObject* parent=Q_NULLPTR):QStyledItemDelegate(parent){
        qDebug()<<"StyleEditorDelegate : StyleEditorDelegate(QObject* parent=Q_NULLPTR)";
        se = new glStyleEditor();
        pl_selected.setBrush(QPalette::Background,QBrush(Qt::lightGray));
        pl_editor.setBrush(QPalette::Background,QBrush(QPalette::Link));
        pl_def.setBrush(QPalette::Background,QBrush(Qt::white));
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const Q_DECL_OVERRIDE{

        if((option.state& QStyle::State_Selected) != 0)
            se->setPalette(pl_selected);
        else
            se->setPalette(pl_def);

        qDebug()<<"void paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index)";
        QObject* p = index.data(Qt::EditRole).value<QObject*>();
        se->getQObject(p);
        se->resize(option.rect.size());
        QPixmap pixmap(option.rect.size());
        se->render(&pixmap);
        painter->drawPixmap(option.rect,pixmap);
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE{
        qDebug()<<"QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index)";
        Q_UNUSED(option);
        //setSubEditorData(m_baseWid,index);
        QObject* p = index.data(Qt::EditRole).value<QObject*>();
        se->getQObject(p);
        return se->sizeHint();
    }

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE Q_DECL_FINAL {
        qDebug()<<"QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)";//<<m_baseWid->winId();
        Q_UNUSED(option);
        Q_UNUSED(index);

        glStyleEditor* editor = new glStyleEditor(parent);
        editor->setPalette(pl_editor);
        //editor->resize(option.rect.size());
        //editor->setParent(parent);
        //setSubEditorData(m_baseWid,index);
        return editor;
    }


    void setEditorData(QWidget* editor, const QModelIndex &index) const Q_DECL_OVERRIDE Q_DECL_FINAL {
        qDebug()<<"StyleEditorDelegate : setEditorData(glStyleEditor * editor, const QModelIndex &index)"<<editor->winId();
        QObject* p = index.data(Qt::EditRole).value<QObject*>();
        glStyleEditor* e = static_cast<glStyleEditor*>(editor);
        Q_ASSERT(e);
        e->getQObject(p);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        qDebug()<<"StyleEditorDelegate : setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)"<<editor->winId();
    glStyleEditor* e = qobject_cast<glStyleEditor*>(editor);
    Q_ASSERT(e);
    model->setData(index, QVariant::fromValue(e->setQObject()), Qt::EditRole);
    }

    void updateEditorGeometry(QWidget* editor,
            const QStyleOptionViewItem& option, const QModelIndex& index) const override
        {
            Q_UNUSED(index);
            editor->setGeometry(option.rect);
        }

private:
glStyleEditor* se;
QPalette pl_selected;
QPalette pl_editor;
QPalette pl_def;
};

#endif // GLSTYLEEDITOR_H
