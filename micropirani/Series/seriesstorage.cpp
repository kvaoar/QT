#include "seriesstorage.h"


SeriesStorage::SeriesStorage(QObject* parent)
    :QObject (parent)
{
    setObjectName("Series");

    series_model = new SeriesTable(this);
    series_model->SetSeriesMap(&series);
    connect(series_model,
            &SeriesTable::color_modified,
            this,
            &SeriesStorage::upd_colortable);
    connect(series_model,
            &SeriesTable::enable_modified,
            this,
           &SeriesStorage::en_modified);

    dialog.setDirectory(QDir::homePath());
    dialog.setNameFilter(FileFilter);
    dialog.setDefaultSuffix("json");
    connect(&dialog,
            &QFileDialog::filterSelected,
            this,
            &SeriesStorage::saveExtChng);
}

SeriesStorage::~SeriesStorage()
{
    qDebug()<<"SeriesStorage::~SeriesStorage()";
    delete series_model;
    qDeleteAll(series);
}

QString SeriesStorage::status(){ return m_status; }


Series* SeriesStorage::addSeries(QString name,Series* val)
{
    if(!series.contains(name)){
        series_model->external_update(true);
            series.insert(name,val);
        series_model->external_update(false);
        emit append(series[name]);
        emit color_changed(series[name]);
    } else {
        qDebug()<<"Series already exist!";
    }

    return series[name];
}

void SeriesStorage::set_series_table(QTableView *ser_table)
{
    series_model->SetTableControls(ser_table);
}





qint64 SeriesStorage::LastTime() const{
    return x_total.max;
}

XInterval SeriesStorage::XTotal(){
    return x_total;
}



QColor SeriesStorage::getColor(QString ser){
    return series.contains(ser)? series.value(ser)->color : QColor(100,100,100);
}

SerIt SeriesStorage::getAt(QString ser, qint64 t) const{
    return series.value(ser)->getAt((t>0)?t:t+LastTime());
}

YInterval SeriesStorage::getYRange(QString ser, XItInterval x){
    return series.value(ser)->getRange(x);
}




void SeriesStorage::write(QJsonObject &json) const
{
    QJsonObject serlist;
    for(QMap<QString,Series*>::const_iterator s = series.begin(); s != series.end(); s++)
    {
        QJsonObject ser;
        s.value()->write(ser);
        serlist.insert(
                    s.key(),
                    ser
                    );
    }
    json["serlist"]= serlist;
}

void SeriesStorage::read(const QJsonObject &json)
{
    if(json.contains("serlist") && json.value("serlist").isObject()){
        QJsonObject serlist = json.value("serlist").toObject();
        for(QJsonObject::Iterator i = serlist.begin(); i != serlist.end(); i++){
            if(i.value().isObject()){
                QString ser_name = i.key();
                QJsonObject ser_object = i.value().toObject();
                Series* s = addSeries(ser_name,new Series (ser_name));
                s->read(ser_object);
                qint64 first = s->physical_values->firstKey();
                qint64 last = s->physical_values->lastKey();
                if(( last > x_total.max)||(x_total.max ==0))
                    x_total.max = last;
                if(( first < x_total.min)||(x_total.min == 0))
                    x_total.min = first;
                //emit refresh(s);
            }
        }
    }
}

QDataStream & operator<<(QDataStream &out, const SeriesStorage & s)
{
    out.setVersion(QDataStream::Qt_5_3);
    out<<s.x_total;
    out<<s.series.keys();
    foreach(Series* ser,s.series.values()){
        out<<*ser;
        qDebug()<<"ser_out";
    }
    return out;
}

QDataStream & operator>>(QDataStream &in, SeriesStorage & s)
{
    in.setVersion(QDataStream::Qt_5_3);
    in>>s.x_total;
    QStringList names;
    in>>names;
    foreach(QString str, names){
        Series* tmp = new Series();
        in>>*tmp;
        s.series.insert(str,tmp);
    }
    return in;
}

void SeriesStorage::Add(QString hostname, SensorState* ss, TimeValPoint p)
{
    //qDebug()<<hostname<<p;
    if(!known_hosts.contains(hostname)){
        known_hosts.append(hostname);
        foreach(QString ser, ss->series_ranges.keys()){
            if(!series.contains(ser)){
                set_status(QString("New series %1 Read AY %2").arg(ser).arg(hostname));
                QPair<qreal,qreal> r = ss->series_ranges[ser];
                QStringList colors = QColor::colorNames();
                YInterval y = {r.first,r.second};
                //30
                addSeries(ser,new Series(ser,colors.at(30+series.count()),y));
                qint64 first = p.t;
                if(( first < x_total.min)||(x_total.min == 0))
                    x_total.min = first;
                    emit last_time(x_total);
            } else {
                set_status("name conflict");
            }
        }
    return;
    }
    foreach(QString ser, p.f.keys()){
        if(!series.contains(ser)){
            set_status("New series in known host! Remove host cache");
            known_hosts.removeOne(hostname);
            return;
        }
        series.value(ser)->physical_values->insert(p.t,p.f[ser]);
        x_total.max = p.t;
        emit last_time(x_total);
        emit lastval(
                    ser,
                    Torr::toString(p.f[ser]),
                    series.value(ser)->color
                    );
        emit refresh(series.value(ser));
    }
}



void SeriesStorage::upd_colortable(QString ser)
{
    emit color_changed(series[ser]);
}

void SeriesStorage::en_modified(QString ser)
{
        emit refresh(series.value(ser));
}

void SeriesStorage::saveExtChng(const QString &s)
{
    if(dialog.acceptMode()== QFileDialog::AcceptMode::AcceptSave){
    if(s.contains("json")) dialog.setDefaultSuffix("json");
    if(s.contains("bin")) dialog.setDefaultSuffix("bin");
    }
}

void SeriesStorage::set_status(QString s)
{
    m_status = s;
    emit statusChanged(s);
}

void SeriesStorage::Load()
{
dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() == QDialog::Accepted){
        QString fileName = dialog.selectedFiles().first();
        QFileInfo fi(fileName);
        if(fi.suffix() == "bin"){
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(nullptr, tr("Unable to open file"),
                    file.errorString());
                return;
            }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_3);
        series_model->external_update(true);
        in>>*this;
        series_model->external_update(false);

        }
        if(fi.suffix() == "json"){
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly| QIODevice::Text)) {
                QMessageBox::information(nullptr, tr("Unable to open file"),
                    file.errorString());
                return;
            }
            QString data = file.readAll();
            file.close();
            QJsonDocument readDoc = QJsonDocument::fromJson(data.toUtf8());
            set_status((readDoc.isNull())?"Not found":"Found data");
            read(readDoc.object());
        }
        emit last_time(x_total);
        foreach(Series* s, series.values()) {
            emit refresh(s);
            emit color_changed(s);
        }
    }
}

void SeriesStorage::Save()
{
dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() == QDialog::Accepted ){
        QString fileName = dialog.selectedFiles().first();
        QString filter = dialog.selectedNameFilter();
        if(filter.contains("json")){
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(nullptr, tr("Unable to open file"),file.errorString());
                return;
            }
           QJsonObject save_obj;
           write(save_obj);
           QJsonDocument saveDoc(save_obj);
           file.write(saveDoc.toJson());
        }
        if(filter.contains("bin")){
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(nullptr, tr("Unable to open file"),file.errorString());
                return;
            }
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_3);
        out<<*this;
        }
    }
}

void SeriesStorage::Reset()
{
    QMessageBox msgBox;
    msgBox.setText("Reset");
    msgBox.setInformativeText("Do you want to remove this data?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok){
        x_total = {0,0};
        foreach(Series* s, series.values()) {
            s->clear();
            emit refresh(s);
        }
        emit last_time(x_total);

    }
}






