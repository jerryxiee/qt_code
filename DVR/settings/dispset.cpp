#include "dispset.h"
#include <QDebug>

DispSet::DispSet(QObject *parent, const QString &filename) : QObject(parent),Config(filename)
{


}

void DispSet::setConfig(int Chn,QString name,bool dispname,bool dispdate,bool dispweek,QString datetype,int value)
{
    Config::setConfig(RootName+QString::number(Chn),ChnName,name);
    Config::setConfig(RootName+QString::number(Chn),DispName,dispname);
    Config::setConfig(RootName+QString::number(Chn),DispDate,dispdate);
    Config::setConfig(RootName+QString::number(Chn),DispWeek,dispweek);
    Config::setConfig(RootName+QString::number(Chn),DateType,datetype);
    Config::setConfig(RootName+QString::number(Chn),TimeType,value);

    preSetConfig(Chn,name, dispname, dispdate,dispweek, datetype, value);
    emit overlaySetPos(Chn);

}

void DispSet::setConfig(const QString &rootName,const QString &name,const QVariant &Value)
{
    Config::setConfig(rootName,name,Value);
}

void  DispSet::preSetConfig(int Chn, QString name, bool dispname, bool dispdate,bool dispweek, QString datetype, int value)
{
    QString time;

    if(dispweek){
        time = "ddd ";
    }
    if(dispdate){
        time.append(datetype);
        if(value == 1){
            time.append(" AP");
        }
    }
    if(dispname){
        qDebug()<<"name:"<<name;
        if(!name.isEmpty()){
            qDebug()<<"emit overlayNameChange";
            emit overlayNameChange(Chn, name);
        }
    }

    if(!time.isEmpty())
        emit overlayTimeTypeChange(time);
}

void DispSet::enableTime(int Chn,bool enable)
{
    Q_UNUSED(Chn);
    QString time;

    if(Config::getConfig(RootName+QString::number(0),DispWeek).toBool()){
        time = "ddd ";
    }
    if(enable){
        time.append(Config::getConfig(RootName+QString::number(0),DateType).toString());
    }

    qDebug()<<"enableTime:"<<enable<<time;
    if(!time.isEmpty())
        emit overlayTimeTypeChange(time);
}

void DispSet::enableWeek(int Chn, bool enable)
{
    Q_UNUSED(Chn);
    QString time;

    if(enable){
        time = "ddd ";
    }
    if(Config::getConfig(RootName+QString::number(0),DispDate).toBool()){
        time.append(Config::getConfig(RootName+QString::number(0),DateType).toString());
    }

    qDebug()<<"enableWeek:"<<enable<<time;
    if(!time.isEmpty())
        emit overlayTimeTypeChange(time);

}

void DispSet::enableName(int Chn, bool enable)
{

}
