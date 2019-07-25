#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <QString>
#include <QList>
#include <QFileInfo>

class VideoFile
{
public:
    VideoFile();
    VideoFile(QString name){mfilename = name;}
//    VideoFile(char *name){VideoFile(QString(name));}

    void setStartIndex(uint value){mstart = value;}
    uint getStartIndex() const{return  mstart;}
    void setEndIndex(uint value){mend = value;}
    uint getEndIndex() const{return  mend;}
    void setCreatTime(uint time){mctime = time;}
    uint getCreatTime() const{return mctime;}
    void setModTime(uint time){mmtime = time;}
    uint getModTime() const{return mmtime;}
    void setSize(uint size){msize = size;}
    uint getSize() const{return msize;}
    void setFrameRate(uint rate) {mframerate = rate;}
    uint getFrameRate() const {return mframerate;}

    void setFileName(QString name){mfilename = name;}
    QString getFileName() const{return mfilename;}
    QString path() const {
        QFileInfo fileinfo(mfilename);
        return fileinfo.path();
    }


public:
    uint mstart;
    uint mend;
    QString mfilename;
    uint mctime;
    uint mmtime;
    uint msize;
    uint mframerate;
};

typedef QList<VideoFile>  VideoFileList;

#endif // VIDEOFILE_H
