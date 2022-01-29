#include "glstyleeditor.h"
#include "ui_glstyleeditor.h"

glStyleEditor::glStyleEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::glStyleEditor)
{
    ui->setupUi(this);
    colors = QColor::colorNames();
    fillComboBox(ui->cbBotCol);
    fillComboBox(ui->cbTopCol);
}

glStyleEditor::~glStyleEditor()
{
    delete ui;
}

void glStyleEditor::getQObject(QObject *obj)
{
    if(obj->property("name").isValid()){
    ui->groupBox->setTitle(obj->property("name").toString());
        if(obj->property("role").isValid()){
            qDebug()<<"role ="<<obj->property("role");
            if(obj->property("role").toInt()==Qt::EditRole)
                editing_obj = obj;
        }

    if(obj->property("color").isValid()) {
        QString color = obj->property("color").toString();
        ui->cbBotCol->setCurrentIndex(colors.indexOf(color));
        QColor c (color);
        c.setAlpha(100);
        ui->cbBotCol->setStyleSheet(QString("background:%1;").arg(c.name())); ;
        ui->wColor->setHidden(false);
    } else {
        ui->wColor->setHidden(true);
    }
    if(obj->property("font").isValid()){
        QFont f = obj->property("font").value<QFont>();
        ui->cbFont->setCurrentFont(f);
        ui->sFontSize->setValue(f.pointSize());
        ui->wFont->setHidden(false);
    } else {
       ui->wFont->setHidden(true);
    }

    }
}

QObject *glStyleEditor::setQObject()
{
QObject* o = new QObject();
o->setProperty("name", ui->groupBox->title());
if(!ui->wColor->isHidden()) o->setProperty("color",QVariant(ui->cbBotCol->currentText()));
if(!ui->wFont->isHidden()) {
    QFont f = ui->cbFont->currentFont();
    f.setPointSize(ui->sFontSize->value());
    o->setProperty("font",QVariant(f));
}
return o;
}


void glStyleEditor::fillComboBox(QComboBox *cb)
{

    cb->addItems(colors);
    foreach(QString col, colors)
        cb->setItemData(colors.indexOf(col),QColor(col),Qt::BackgroundRole);
}


