#include "logtabdata.h"
#include <QDebug>
#include <QDateTime>

LogTabData *LogTabData::mLogRecord = nullptr;
QMap<CLOGTYPE,QString>LogTabData::mLogType;
QMap<MLOGTYPE,QString> LogTabData::mLogMType;

LogTabData * LogTabData::getLogRecoedPoint()
{
    if(mLogRecord == nullptr){
        mLogRecord = new LogTabData();

        if(!SqliteDateBase::getSqliteDateBase()->open()){
            delete mLogRecord;
            return nullptr;
        }
        qDebug()<<"logdatabase open success";
        if(!mLogRecord->isTabExists()){
            if(mLogRecord->createTable()){
                qDebug()<<"create logdbtab success";
            }else {
                qDebug()<<"create logdbtab failed";
                delete mLogRecord;
                return nullptr;
            }
        }

        mLogRecord->setLogId(mLogRecord->count());
    }

    return mLogRecord;
}

LogTabData::LogTabData(QObject *parent) : QObject(parent),mLogId(0)
{
    mLogType.clear();
    mLogMType.clear();

    mLogMType.insert(ALARM,"报警");
    mLogMType.insert(INFO,"信息");
    mLogMType.insert(EXCEPT,"异常");
    mLogMType.insert(OPERATE,"操作");

    mLogType.insert(STRECORD,"开始录像");
    mLogType.insert(ENDRECORD,"停止录像");
    mLogType.insert(SYSTEMSTART,"开机");
    mLogType.insert(SYSTEMEND,"关机");
    mLogType.insert(REBOOT,"重启");
    mLogType.insert(PLATFORMMSG,"平台消息");
    mLogType.insert(LOGON,"登录");
    mLogType.insert(LOGOUT,"退出登录");
    mLogType.insert(LOCALCFG,"本地配置");
    mLogType.insert(REMOTECFG,"远程配置");
    mLogType.insert(MOTODET,"动测");
    mLogType.insert(ALARMIN,"输入报警");
    mLogType.insert(ALARMOUT,"报警输出");
    mLogType.insert(ALARMREPORT,"报警上报");

}

LogTabData::~LogTabData()
{
    if(mLogRecord)
        delete mLogRecord;
}

bool LogTabData::createTable()
{
//    QSqlQuery query;
    QString tabinfo = "(ID INTEGER PRIMARY KEY NOT NULL, \
            CH TEXT , \
            MainType TEXT NOT NULL, \
            SecType TEXT NOT NULL, \
            CreateT INTEGER NOT NULL, \
            Describe TEXT)";

    return SqliteDateBase::getSqliteDateBase()->createTable(LOGDBTAB,tabinfo);
}

bool LogTabData::isTabExists()
{
//    mLogId = countInTab(LOGDBTAB);
    return SqliteDateBase::getSqliteDateBase()->isTabExists(LOGDBTAB);
}

bool LogTabData::setLogId(quint64 logid)
{
    mLogId = logid;

    return true;
}

int LogTabData::count()
{
    return SqliteDateBase::getSqliteDateBase()->countInTab(LOGDBTAB);
}

bool LogTabData::insertSignalData(DateInfo &info)
{
    QSqlQuery query;

    query.prepare("INSERT INTO "+LOGDBTAB+" VALUES(:ID,:CH,:MainType,:SecType,:CreateT,:Describe)");
    query.bindValue(":ID",info.mId);
    if(info.mChn < 0)
        query.bindValue(":CH",QVariant());
    else {
        query.bindValue(":CH","CH"+QString::number(info.mChn));
    }
    query.bindValue(":MainType",info.mType);
    query.bindValue(":SecType",info.mType1);
    query.bindValue(":CreateT",info.mCreateTime);
    if(info.mCommit.isNull())
        query.bindValue(":Describe",QVariant());
    else {
        query.bindValue(":Describe",info.mCommit);
    }


    if(query.exec()){
        return true;
    }

    qDebug()<<"insertSignalData failed "<<query.lastError();

    return false;

}

bool LogTabData::insertMorelData(QList<DateInfo> &infolist)
{
    QSqlQuery query;
    QVariantList idlist;
    QVariantList Chnist;
    QVariantList typeist;
    QVariantList type1ist;
    QVariantList timeist;
    QVariantList commitist;

    query.prepare("INSERT INTO "+LOGDBTAB+" VALUES(?,?,?,?,?,?)");

    for (int i = 0;i < infolist.count();i++) {
        idlist<<infolist.at(i).mId;
        Chnist<<infolist.at(i).mChn;
        typeist<<infolist.at(i).mType;
        type1ist<<infolist.at(i).mType1;
        timeist<<infolist.at(i).mCreateTime;
        commitist<<infolist.at(i).mCommit;
    }

    query.addBindValue(idlist);
    query.addBindValue(Chnist);
    query.addBindValue(typeist);
    query.addBindValue(type1ist);
    query.addBindValue(timeist);
    query.addBindValue(commitist);

    if(query.execBatch())
        return true;

    qDebug()<<query.lastError();

    return false;
}

bool LogTabData::writeOneData(MLOGTYPE mtype,CLOGTYPE ctype,QString commit,int Chn )
{
    DateInfo info;

    info.mId = mLogId++;
    info.mChn = Chn;
    info.mType = mLogMType.value(mtype,"N");
    info.mType1 = mLogType.value(ctype,"N");
    info.mCommit = commit;
    info.mCreateTime = QDateTime::currentDateTime().toTime_t();

    return insertSignalData(info);
}
