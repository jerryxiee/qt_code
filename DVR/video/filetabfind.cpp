#include "filetabfind.h"
#include <QDebug>
#include <QDateTime>

//FileTabFind::FileTabFind()
//{

//}

FileTabFind * FileTabFind::openFileTab(int Chn)
{
    const QString INDEXPATH = "/mnt/sda1/venc/channel";
    const QString INDEXNAME = "/file.index";

    QString indexfile = INDEXPATH+QString::number(Chn) + INDEXNAME;

    QFileInfo fileinfo(indexfile);
    if(!fileinfo.exists()){
        return nullptr;
    }


    return new FileTabFind(indexfile);
}

FileTabFind * FileTabFind::createNewTab(int Chn)
{
    const QString INDEXPATH = "/mnt/sda1/venc/channel";
    const QString INDEXNAME = "/file.index";
    bool isNew = false;

    QString indexfile = INDEXPATH+QString::number(Chn) + INDEXNAME;

    QFileInfo fileinfo(indexfile);
    if(!fileinfo.exists()){
        isNew = true;
    }

    FileTabFind * filetab = new FileTabFind(indexfile);
    if(isNew){
        filetab->resetTabFile(Chn);
    }

    return filetab;
}

FileTabFind::FileTabFind(QString filename)
{
//    mFileName = filename;
    mFile.setFileName(filename);
    if(!mFile.open(QIODevice::ReadWrite)){
        qDebug()<<"open tabfile error";
    }


}

//void FileTabFind::setFileName(QString filename)
//{
////    mFileName = filename;
//    mFile.setFile(filename);
//}

//QString FileTabFind::getFileName() const
//{
////    return mFileName;
//    return mFile.fileName();
//}

//QString FileTabFind::absoluteFilePath() const
//{
//    return mFile.absoluteFilePath();
//}
int FileTabFind::getFileLastDayIndex()
{
    INDEXTABHEAD head;

    mFile.seek(0);
    if(mFile.read((char *)&head,sizeof (INDEXTABHEAD)) < 0){
        mFile.seek(mFile.size());
        return -1;
    }

    mFile.seek(mFile.size());

    return head.durationDays;

}

int FileTabFind::getCurDayIndex()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString dateStr = current_date_time.toString("yyyy-MM-dd");
    uint time = QDateTime::fromString(dateStr,"yyyy-MM-dd").toTime_t();
    INDEXTABHEAD head;

    mFile.seek(0);
    if(mFile.read((char *)&head,sizeof (INDEXTABHEAD)) < 0){
        mFile.seek(mFile.size());
        return -1;
    }
    mFile.seek(mFile.size());

    return (time - head.time) /(3600*24);
}

int FileTabFind::getDayIndex(uint timevalue)
{
    INDEXTABHEAD head;

    mFile.seek(0);
    if(mFile.read((char *)&head,sizeof (INDEXTABHEAD)) < 0){
        mFile.seek(mFile.size());
        return -1;
    }
    mFile.seek(mFile.size());

    return (timevalue - head.time)/(3600*24);
}

int FileTabFind::getCurFileTabIndex()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString dateStr = current_date_time.toString("yyyy-MM-dd");
    uint time = QDateTime::fromString(dateStr,"yyyy-MM-dd").toTime_t();
//    int filetabindex;

    return  (current_date_time.toTime_t() - time)/(3600/SPLITNUM);


}

int FileTabFind::getFileTabIndex(uint timevalue)
{
    int days = timevalue % (3600*24);
    qDebug()<<"days:"<<days;

    return  (days)/(3600/SPLITNUM);
}

uint FileTabFind::getFileIndex()
{
    if(mFile.isReadable()){
        INDEXTABHEAD head;
        mFile.seek(0);
        if(mFile.read((char *)&head,sizeof (INDEXTABHEAD)) > 0){
            mFile.seek(mFile.size());
            qDebug()<<"getfile index:"<<head.curindex;
            return head.curindex;
        }
    }
    mFile.seek(mFile.size());
    return 0;
}

bool FileTabFind::writeFileIndex(uint index)
{
    if(mFile.isReadable()){
        INDEXTABHEAD head;
        mFile.seek(0);
        if(mFile.read((char *)&head,sizeof (INDEXTABHEAD)) > 0){
            head.curindex = index+1;
            mFile.seek(0);
            mFile.write((char *)&head,sizeof (INDEXTABHEAD));
            mFile.seek(mFile.size());
            qDebug()<<"getfile index:"<<head.curindex;
            mFile.flush();
            return true;
        }
    }
    mFile.seek(mFile.size());

    return false;

}

bool FileTabFind::getDayTab(DAYTABNODE &daytab,uint timevalue)
{
    DATATABNODE datnode;
//    DAYTABNODE daynode;

    int dayindex = getDayIndex(timevalue);
    int filetabindex = getFileTabIndex(timevalue);

    off_t offset = sizeof (INDEXTABHEAD) + dayindex*sizeof (DATATABNODE);

    mFile.seek(offset);
    if(mFile.read((char *)&datnode,sizeof (DATATABNODE)) > 0){
        if(datnode.offset != 0){
            mFile.seek(datnode.offset + filetabindex*sizeof (DAYTABNODE));
            mFile.read((char *)&daytab,sizeof (DAYTABNODE));
            mFile.seek(mFile.size());
            return true;
        }

    }

    mFile.seek(mFile.size());
    return false;
}

bool FileTabFind::writeDayTab(DAYTABNODE &daytab,uint timevalue)
{
    DATATABNODE datnode;
//    DAYTABNODE daynode;

    int dayindex = getDayIndex(timevalue);
    int filetabindex = getFileTabIndex(timevalue);
    qDebug()<<"write filetabindex:"<<filetabindex;

    off_t offset = sizeof (INDEXTABHEAD) + dayindex*sizeof (DATATABNODE);

    mFile.seek(offset);
    if(mFile.read((char *)&datnode,sizeof (DATATABNODE)) > 0){
        if(datnode.offset != 0){
            mFile.seek(datnode.offset + filetabindex*sizeof (DAYTABNODE));
            mFile.write((char *)&daytab,sizeof (DAYTABNODE));
            mFile.seek(mFile.size());
            mFile.flush();
            return true;
        }

    }

    mFile.seek(mFile.size());
    return false;

}
bool FileTabFind::checkDayTabExist(uint index)
{
    if(mFile.isReadable()){
        DATATABNODE datnode;
        mFile.seek(sizeof (INDEXTABHEAD) + index*sizeof (DATATABNODE));
        if(mFile.read((char *)&datnode,sizeof (DATATABNODE)) > 0){
            mFile.seek(mFile.size());
            if(datnode.offset != 0){
                return true;
            }
        }

    }

    mFile.seek(mFile.size());
    return false;
}

bool FileTabFind::createNewDayTab()
{
    if(!mFile.isWritable()){
        return false;
    }

    INDEXTABHEAD head;
    DATATABNODE datnode;
    int dayindex;

    dayindex = getCurDayIndex();
    if(dayindex < 0){
        qDebug()<<"get current day index error";
        return false;
    }

    if(checkDayTabExist(dayindex)){
        qDebug()<<"DayTab exist";
        return false;
    }

    mFile.seek(0);
    if(mFile.read((char *)&head,sizeof (INDEXTABHEAD)) < 0){
        goto error;
    }
    mFile.seek(0);
    head.durationDays = dayindex;
    mFile.write((char *)&head,sizeof (INDEXTABHEAD));

    mFile.seek(dayindex*sizeof (DATATABNODE) + sizeof (INDEXTABHEAD));
    if(mFile.read((char *)&datnode,sizeof (DATATABNODE)) < 0){
        goto error;
    }
    if(datnode.offset == 0){
        datnode.offset = mFile.size();
        mFile.seek(mFile.pos() - sizeof (DATATABNODE));
        mFile.write((char *)&datnode,sizeof (DATATABNODE));
        mFile.seek(mFile.size());

        DAYTABNODE daynode;
        daynode.num = 0;
        daynode.offset = 0;
        for (int i = 0;i < 24*SPLITNUM;i++) {
            mFile.write((char *)&daynode,sizeof (DAYTABNODE));
        }
        mFile.flush();
    }

    return true;

error:
    mFile.seek(mFile.size());
    return false;
}

bool FileTabFind::addFileToTab(FILENODE & filenode)
{
    off_t offset = mFile.size();

    if(mFile.isWritable()){
        if(mFile.write((char *)&filenode,sizeof (FILENODE)) > 0){
//            INDEXTABHEAD head;
//            DATATABNODE daynode;
            DAYTABNODE daytab;

            if(!getDayTab(daytab,filenode.stTime)){
                qDebug()<<"getDayTab error";
                return false;
            }

            if(daytab.num == 0){
                daytab.offset = offset;
            }
            daytab.num++;

            if(!writeDayTab(daytab,filenode.stTime)){
                qDebug()<<"writeDayTab error";
                return false;
            }
            if(!writeFileIndex(filenode.fileindex)){
                qDebug()<<"writeFileIndex error";
                return false;
            }
//            int dayindex = getDayIndex(filenode.stTime);
//            int filetabindex = getFileTabIndex(filenode.stTime);
//            off_t offset = sizeof (INDEXTABHEAD) + dayindex*sizeof (DATATABNODE);

//            mFile.seek(offset);
//            if(mFile.read((char *)&daynode,sizeof (DATATABNODE)) < 0){
//                qDebug()<<"read daynode error";
//                mFile.seek(mFile.size());
//                return false;
//            }

//            offset = daynode.offset;

        }
    }

    qDebug()<<"add file to tab";
    return true;
}

bool FileTabFind::resetTabFile(int Chn)
{
    Q_UNUSED(Chn)
    char buf[1024];

    memset(buf,0x0,sizeof (buf));

    if(mFile.isWritable()){
        mFile.reset();
        for (int i = 0;i < 1024;i++) {
            if(!mFile.write(buf,sizeof (buf))){
                qDebug()<<"filetab reset error";
                return false;
            }
        }
        mFile.seek(0);
//        unsigned int time = QDateTime::currentDateTime()
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString dateStr = current_date_time.toString("yyyy-MM-dd");
        uint time = QDateTime::fromString(dateStr,"yyyy-MM-dd").toTime_t();
        INDEXTABHEAD head;
        head.time = time;
        head.curindex = 0;
        head.durationDays = 0;
        mFile.write((char *)&head,sizeof (INDEXTABHEAD));

        mFile.seek(sizeof (INDEXTABHEAD));
        DATATABNODE datetab;
        datetab.offset = INDEXDATTABSIZE;
        mFile.write((char *)&datetab,sizeof (DATATABNODE));
        mFile.seek(INDEXDATTABSIZE);

        DAYTABNODE daynode;
        daynode.num = 0;
        daynode.offset = 0;
        for (int i = 0;i < 24*SPLITNUM;i++) {
            mFile.write((char *)&daynode,sizeof (DAYTABNODE));
        }
        mFile.flush();

    }
    return true;
}

void FileTabFind::close()
{
    if(mFile.isOpen()){
        mFile.close();
    }

}
