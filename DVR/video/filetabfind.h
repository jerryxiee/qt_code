#ifndef FILETABFIND_H
#define FILETABFIND_H

#include <QString>
#include <QFileInfo>
#include "hi_common.h"

class FileTabFind
{
public:
    static FileTabFind * createNewTab(int Chn);
    static FileTabFind * openFileTab(int Chn);

    bool resetTabFile(int Chn);  //重新初始化文件表
    bool checkDayTabExist(uint index);   //检查
    bool createNewDayTab();         //有录像，则建立当天录像索引表
    bool getDayTab(DAYTABNODE &daytab,uint timevalue);
    bool writeDayTab(DAYTABNODE &daytab,uint timevalue);
    bool addFileToTab(FILENODE & filenode);       //将录像文件索引增加到表中
    int getCurDayIndex();      //
    int getFileLastDayIndex();
    int getDayIndex(uint timevalue);
    int getCurFileTabIndex();   //获取当前时间所在索引表位置
    int getFileTabIndex(uint timevalue);
    uint getFileIndex();         //获取MP4文件索引号
    bool writeFileIndex(uint index);
    void close();
protected:
//    FileTabFind();
    FileTabFind(QString filename);

//    void setFileName(QString filename);

//    QString getFileName() const;
//    QString absoluteFilePath() const;



private:
    QFile mFile;
//    QString mFileName;
};

#endif // FILETABFIND_H
