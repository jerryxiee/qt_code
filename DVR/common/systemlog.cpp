#include "systemlog.h"
#include <QDebug>
#include <QDateTime>
#include <stdarg.h>
#include <stdio.h>
#include <QDir>
#include <QFileInfo>

SystemLog *SystemLog::mSystemLog = nullptr;

SystemLog::SystemLog(QObject *parent,QString logname) : QObject(parent)
{
    mlogfile.setFileName(logname);
    QFileInfo logfileinfo(mlogfile);
    QDir logpath(logfileinfo.path());
    if(!logpath.exists()){
        if(!logpath.mkpath(logfileinfo.path())){
            qDebug()<<"mkpath %s failed\n"+logfileinfo.path();
            return;
        }
    }
    mlogfile.open(QIODevice::WriteOnly|QIODevice::Text);
    if(!mlogfile.isOpen()){
        qDebug()<<"can not open logfile";
    }
}

SystemLog::~SystemLog()
{
    if(mlogfile.isOpen()){
        mlogfile.close();
    }
}

SystemLog *SystemLog::getSystemLog()
{
    if(mSystemLog == nullptr){
        mSystemLog = new SystemLog();
    }

    return mSystemLog;
}

void SystemLog::logwrite(int priority, QString str)
{
    Q_UNUSED(priority)

    if(mlogfile.size() > LOGMAXSIZE){
        mlogfile.seek(0);
    }
    if(!mlogfile.isOpen()){
        return;
    }

    QTextStream logout(&mlogfile);
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");

    logout <<datetime+": "+str<<endl;

}

void SystemLog::logwrite(int priority,const char *fmt,...)
{
    Q_UNUSED(priority)
    char buff[1024];

    va_list args;
    va_start(args, fmt);
    vsprintf(buff,fmt,args);
    va_end(args);

    logwrite(priority, QString(buff));
}
