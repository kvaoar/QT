#include "ftstream.h"
#include<QFileDialog>
#include<QTextStream>
#include<QMessageBox>
#include<QDebug>

FTStream::FTStream(QObject *parent) : QObject(parent)
{

}

void FTStream::add(TimeValPoint p){
    points.append(p);
    emit refresh(&points);
}

void FTStream::Save()
{
    QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save datastream"), tr(""), tr("Data (*.csv);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(NULL, tr("Unable to open file"),file.errorString());
            return;
        }
        QTextStream out(&file);
        for(QList<TimeValPoint>::const_iterator i = points.begin(); i != points.end(); ++i)
            out << QDateTime::fromMSecsSinceEpoch(i->t).toString("dd-MM-yyyy hh:mm:ss.zzz")<<","<<QString::number(i->f)<<";\r\n";
    }
}

void FTStream::Load()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, tr("Load datastream"), tr(""), tr("Data (*.csv);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(NULL, tr("Unable to open file"),
            file.errorString());
        return;
    }
    points.clear();
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        QStringList l = line.split(QRegExp("\\,|\\;|\\\r|\\\n"),QString::SkipEmptyParts);
        points.append(TimeValPoint{QDateTime::fromString(l.at(0),"dd-MM-yyyy hh:mm:ss.zzz").toMSecsSinceEpoch(),l.at(1).toFloat()});
       // qDebug()<<"load"<<l.at(0)<<","<<l.at(1)<<";"<<points.size()<<" val"<<points.last().f<<"  "<<QDateTime::fromMSecsSinceEpoch( points.last().t).toString("dd-MM-yyyy hh:mm:ss.zzz t");
        line = in.readLine();
    }
    emit refresh(&points);
 }
}

void FTStream::Reset()
{
    QMessageBox msgBox;
    msgBox.setText("Reset");
    msgBox.setInformativeText("Do you want to remove this data?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok) points.clear();
}
