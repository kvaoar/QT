#include "mcmdline.h"
#include<QDebug>
MCmdLine::MCmdLine(QWidget *parent)
: QLineEdit(parent)
{
    history.append("");
    history_point = 0;
}

MCmdLine::MCmdLine(const QString &text, QWidget *parent)
: QLineEdit(text, parent)
{

}

MCmdLine::~MCmdLine()
{

}


bool MCmdLine::event(QEvent *event)
{
    if(!isEnabled())return QWidget::event(event);
    if (event->type() == QEvent::KeyPress) {
       QKeyEvent *keyEvent = (QKeyEvent *)event;

       int key = keyEvent->key();
       switch (key) {

       case Qt::Key_Tab:
           qDebug()<< "Tab"<< complete_pos;
           if(this->text().length()>0){
               wait_complete = true;
           emit tabPressed(this->text().right(this->text().length() - complete_pos)+'\t');
           }
           return true;
           break;
       case Qt::Key_Return:
       case Qt::Key_Enter:
          // qDebug()<< "Enter"<< complete_pos;
           history.append(this->text());
          // qDebug()<< this->text()<< history.count();
           if(history.count()>history_length) history.removeAt(0);
           history_point = history.count();
           wait_complete = false;
           emit enterPressed((this->text()).right(this->text().length() - complete_pos)+'\n');
           complete_pos = 0;
           this->setText("");
           return true;
           break;
       case Qt::Key_Up:
           complete_pos = 0;
           history_point--;
           if(history_point <0) history_point = 0;
           this->setText(history.at(history_point));
          // qDebug()<< history_point<< history.count();
            return true;
           break;
       case Qt::Key_Down:
           complete_pos = 0;
           history_point++;
           if(history_point >= (history.count())){ history_point = history.count(); this->setText("");}
           if(history_point < history.count())
               this->setText(history.at(history_point));
                   //  qDebug()<< history_point<< history.count();
           return true;
           break;
       case Qt::Key_Escape:
           complete_pos = 0;
           setText("");
           return true;
           break;
       default:
            QWidget::event(event);
           break;

       }

     }else
     return QWidget::event(event);
}

int MCmdLine::getHistoryLen()
{
    return history_length;
}

void MCmdLine::setHistoryLen(int l)
{
    if(l <= HISTORY_MAX) history_length = l;
}

void MCmdLine::cmdComplete(QString s)
{
    if(wait_complete){
    setText(s);
    complete_pos = s.length();
    //qDebug()<<"cmdComplete"<<s<<complete_pos;
    }
}
