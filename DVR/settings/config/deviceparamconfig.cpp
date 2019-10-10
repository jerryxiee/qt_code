#include "deviceparamconfig.h"
#include <QDebug>

DeviceParamConfig *DeviceParamConfig::mDeviceConfig = nullptr;


DeviceParamConfig *DeviceParamConfig::getDeviceConfig()
{
    if(!mDeviceConfig){
        mDeviceConfig = new DeviceParamConfig();
        if(!mDeviceConfig){
            qDebug()<<"create DeviceParamConfig error!";
            return nullptr;
        }

        if(!mDeviceConfig->open()){
            delete mDeviceConfig;
            return nullptr;
        }

        if(!mDeviceConfig->isTabExists()){
            if(!mDeviceConfig->createTable()){
                qDebug()<<"DeviceConfig createTable failed";
                delete mDeviceConfig;
                return nullptr;
            }
        }
    }

    return mDeviceConfig;
}

DeviceParamConfig::DeviceParamConfig(QObject *parent) : QObject(parent)
{

}

bool DeviceParamConfig::createTable()
{
    QString tabinfo = "(\
            ID INTEGER NOT NULL, \
            NUM INTEGER NOT NULL, \
            FIELD INTEGER NOT NULL, \
            VALUE TEXT)";

    return SqliteDateBase::createTable(TABNAME,tabinfo);

}

bool DeviceParamConfig::isTabExists()
{
    return SqliteDateBase::isTabExists(TABNAME);
}

bool DeviceParamConfig::insertSignalData(DeviceParam &param)
{
    QSqlQuery query;

    query.prepare("INSERT INTO "+TABNAME+" VALUES(:ID,:NUM,:FIELD,:VALUE)");
    query.bindValue(":ID",param.id);

    query.bindValue(":NUM",param.num);
    query.bindValue(":FIELD",param.field);
    query.bindValue(":VALUE",param.value);

    if(query.exec()){
        return true;
    }

    qDebug()<<"insertSignalData failed "<<query.lastError();

    return false;

}

bool DeviceParamConfig::insertMorelData(DeviceParamList &paramlist)
{
    QSqlQuery query;
    QVariantList idlist;
    QVariantList numlist;
    QVariantList typelist;
    QVariantList valuelist;

    query.prepare("INSERT INTO "+TABNAME+" VALUES(?,?,?,?)");

    for (int i = 0;i < paramlist.count();i++) {
        idlist<<paramlist.at(i).id;
        numlist<<paramlist.at(i).num;
        typelist<<paramlist.at(i).field;
        valuelist<<paramlist.at(i).value;

    }

    query.addBindValue(idlist);
    query.addBindValue(numlist);
    query.addBindValue(typelist);
    query.addBindValue(valuelist);

    if(query.execBatch())
        return true;

    qDebug()<<query.lastError();

    return false;

}

bool DeviceParamConfig::updateData(int id, int num, int field, QString value)
{
    QString setvalue = "VALUE="+value;
    QString where = "ID="+QString::number(id)+",NUM="+QString::number(num)+",FIELD="+QString::number(field);

    return SqliteDateBase::updateTabData(TABNAME,setvalue,where);

}

QString DeviceParamConfig::getValue(int id,int num,int field)
{
    QSqlQuery query;
    QString where = "ID="+QString::number(id)+",NUM="+QString::number(num)+",FIELD="+QString::number(field);

    if(!SqliteDateBase::getTabData(query,TABNAME,where)){
        qDebug()<<"getValue failed";
        return QString();
    }

    if(query.next()){
        return query.value("VALUE").toString();
    }

    return QString();
}


QStringList DeviceParamConfig::getValue(int id,int num)
{
    QSqlQuery query;
    QStringList list;
    QString where = "ID="+QString::number(id)+",NUM="+QString::number(num);

    if(!SqliteDateBase::getTabData(query,TABNAME,where)){
        qDebug()<<"getValue failed";
        return list;
    }

    while(query.next()){
        list.push_back(query.value("VALUE").toString());
    }

    return list;
}
