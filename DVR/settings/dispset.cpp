#include "dispset.h"
#include <QDebug>
#include <QRect>

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
    emit rgnOverlayShow(Chn);
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

void DispSet::setNameRect(int Chn,QRect &rect)
{
    Config::setConfig(RootName+QString::number(Chn),NamePos,rect);
}

QRect &DispSet::getNameRect(int Chn, QRect &rect)
{
    rect = Config::getConfig(RootName+QString::number(Chn),NamePos).toRect();

    return rect;
}

void DispSet::setTimeRect(int Chn, QRect &rect)
{
    Config::setConfig(RootName+QString::number(Chn),TimePos,rect);
}

QRect &DispSet::getTimeRect(int Chn, QRect &rect)
{
    rect = Config::getConfig(RootName+QString::number(Chn),TimePos).toRect();

    return rect;
}

void DispSet::setTimeShowEnabled(int Chn, bool enable)
{
    Config::setConfig(RootName+QString::number(Chn),DispDate,enable);
}

bool DispSet::getTimeShowEnabled(int Chn )
{
    return Config::getConfig(RootName+QString::number(Chn),DispDate).toBool();
}

void DispSet::setNameShowEnabled(int Chn, bool enable)
{
    Config::setConfig(RootName+QString::number(Chn),DispName,enable);
}
bool DispSet::getNameShowEnabled(int Chn)
{
    return Config::getConfig(RootName+QString::number(Chn),DispName).toBool();
}
void DispSet::setWeekShowEnabled(int Chn, bool enable)
{
    Config::setConfig(RootName+QString::number(Chn),DispWeek,enable);
}
bool DispSet::getWeekShowEnabled(int Chn)
{
    return Config::getConfig(RootName+QString::number(Chn),DispWeek).toBool();
}

void DispSet::setTimeType(int Chn, QString &type)
{
    Config::setConfig(RootName+QString::number(Chn),TimeType,type);
}

QString &DispSet::getTimeType(int Chn, QString &type)
{
    type = Config::getConfig(RootName+QString::number(Chn),TimeType).toString();

    return type;
}

void DispSet::setDateType(int Chn, int type)
{
    Config::setConfig(RootName+QString::number(Chn),DateType,type);
}

int DispSet::getDateType(int Chn)
{
    return Config::getConfig(RootName+QString::number(Chn),DateType).toInt();
}
