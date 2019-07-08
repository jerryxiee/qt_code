#include "config.h"
#include <QDebug>

Config::Config():mpSetting(nullptr)
{
    mFileName = ".config";
    mpSetting = new QSettings(CONFIG_PATH+mFileName,QSettings::IniFormat);
}

Config::Config(const QString &filename)
{
    mFileName = filename;

    mpSetting = new QSettings(CONFIG_PATH+mFileName,QSettings::IniFormat);
}

Config::~Config()
{
    delete mpSetting;
//    mpSetting = nullptr;
}


void Config::setFileName(const QString &filename)
{
    mFileName = filename;
    if(mpSetting){
        delete mpSetting;
    }

    mpSetting = new QSettings(CONFIG_PATH+mFileName,QSettings::IniFormat);
}

void Config::setConfig(const QString &rootName,const QString &name,const QVariant &Value)
{
    mpSetting->setValue(QString("/%1/%2").arg(rootName).arg(name),Value);

}

QVariant Config::getConfig(const QString &rootName,const QString &name) const
{
    return mpSetting->value(QString("/%1/%2").arg(rootName).arg(name));
}
