#include "platformconfigini.h"

PlatformConfigIni::PlatformConfigIni(QObject *parent, QString filename) : QObject(parent),Config (filename)
{

}

bool PlatformConfigIni::insertSignalData(DeviceParam &param)
{
    QString key = QString::number(param.id)+QString::number(param.num)+QString::number(param.field);

    setConfig(ROOTNAME,key,param.value);

    return true;
}

QVariant PlatformConfigIni::getValue(int id,int num,int field) const
{
    QString key = QString::number(id)+QString::number(num)+QString::number(field);

    return getConfig(ROOTNAME,key);

}



