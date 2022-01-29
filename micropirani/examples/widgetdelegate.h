#ifndef WIDGETDELEGATE_H
#define WIDGETDELEGATE_H
#include <QStyledItemDelegate>
#include <QPainter>
#include <QDebug>
#include <QLabel>
template <class T>
class WidgetDelegate : public QStyledItemDelegate{
#ifdef Q_COMPILER_STATIC_ASSERT
    static_assert(std::is_base_of<QWidget,T>::value,"Template argument must be a QWidget");
#endif
    Q_DISABLE_COPY(WidgetDelegate)
public:
    explicit WidgetDelegate(QObject* parent = Q_NULLPTR)
        :QStyledItemDelegate(parent)
        , m_baseWid(new T)
    { m_baseWid->setProperty("view",true);}
    ~WidgetDelegate(){
        delete m_baseWid;
    }
    void paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const Q_DECL_OVERRIDE{
        qDebug()<<"void paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index)"<<m_baseWid->winId();
        setSubEditorData(m_baseWid,index);
        m_baseWid->resize(option.rect.size());
        //m_baseWid->render(painter);
        QPixmap pixmap(option.rect.size());
        m_baseWid->render(&pixmap);
        painter->fillRect(option.rect, option.palette.highlight());
        painter->drawPixmap(option.rect,pixmap);
        painter->drawRect(option.rect);
    }
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE{
        qDebug()<<"QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index)"<<m_baseWid->winId();
        Q_UNUSED(option);
        //setSubEditorData(m_baseWid,index);
        return m_baseWid->sizeHint();
    }
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE Q_DECL_FINAL {
        qDebug()<<"QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)"<<m_baseWid->winId();
        Q_UNUSED(option);
        Q_UNUSED(index);
        T* editor = new T(parent);
        //editor->setParent(parent);
        //setSubEditorData(m_baseWid,index);
        return editor;
    }
    virtual void setSubEditorData(T* editor, const QModelIndex &index) const =0;

    void setEditorData(QWidget* editor, const QModelIndex &index) const Q_DECL_OVERRIDE Q_DECL_FINAL {
        qDebug()<<"void setEditorData(QWidget* editor, const QModelIndex &index) "<<m_baseWid->winId();
        T* subEdit = qobject_cast<T*>(editor);
        Q_ASSERT(subEdit);
        return setSubEditorData(subEdit,index);
    }


   /* void updateEditorGeometry(QWidget* editor,
        const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        qDebug()<<"geometry";
        Q_UNUSED(index);
        editor->setGeometry(option.rect);
    }*/

private:
    T* m_baseWid;
};


#endif // WIDGETDELEGATE_H
