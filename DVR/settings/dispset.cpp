#include "dispset.h"

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

}
