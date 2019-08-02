#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <QString>
#include <QList>
#include <QFileInfo>
#include "common/sample_common_sys.h"

class VideoFile
{
public:
    VideoFile();
    VideoFile(QString name){mfilename = name;}
//    VideoFile(char *name){VideoFile(QString(name));}

    void setStartIndex(uint value){mstart = value;}
    uint getStartIndex() const{return  mstart;}
    void setStartOffset(uint value){mstoffset = value;}
    uint getStartOffset() const{return  mstoffset;}
    void setEndOffset(uint value){mendoffset = value;}
    uint getEndOffset() const{return  mendoffset;}
    void setEndIndex(uint value){mend = value;}
    uint getEndIndex() const{return  mend;}
    void setCreatTime(uint time){mctime = time;}
    uint getCreatTime() const{return mctime;}
    void setModTime(uint time){mmtime = time;}
    uint getModTime() const{return mmtime;}
    void setSize(uint size){msize = size;}
    uint getSize() const{return mendoffset - mstoffset;}
    void setFrameRate(uint rate) {mframerate = rate;}
    uint getFrameRate() const {return mframerate;}
    void setWidth(uint value) {mwidth = value;}
    uint getWidth() const {return mwidth;}
    void setHight(uint value) {mhight = value;}
    uint getHight() const {return mhight;}
    void setEnType(PAYLOAD_TYPE_E type) {enType = type;}
    PAYLOAD_TYPE_E getEnType() const {return enType;}

    void setFileName(QString name){mfilename = name;}
    QString getFileName() const{return mfilename;}
    QString path() const {
        QFileInfo fileinfo(mfilename);
        return fileinfo.path();
    }


public:
    uint mwidth;
    uint mhight;
    uint mstart;
    uint mend;
    QString mfilename;
    uint mctime;
    uint mmtime;
    uint msize;
    uint mframerate;
    uint mstoffset;
    uint mendoffset;
    PAYLOAD_TYPE_E enType;
};

typedef QList<VideoFile>  VideoFileList;

#endif // VIDEOFILE_H
