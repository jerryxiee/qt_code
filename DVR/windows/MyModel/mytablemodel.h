#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QFileInfoList>
#include <QDir>
#include <QStack>
#include <QQmlListProperty>
#include "common/sample_comm.h"
#include "video/videosearch.h"
#include <QtConcurrent/QtConcurrentRun>
#ifndef LUNUX_WIN
#include "video/mp4fileindex.h"
#endif

class MyTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList roles READ roles WRITE setRoles)
    Q_PROPERTY(QString name READ name NOTIFY fileNameChanged)
    Q_PROPERTY(QString mCurrentPath READ pathname NOTIFY pathChanged)
//    Q_PROPERTY(QQmlListProperty<QFileInfo> mFileInfoList READ getFileInfoList)
public:
    explicit MyTableModel(QObject *parent = nullptr);
    ~MyTableModel();

    void showVideoFileList(VideoFileList &list);
    void showVideoFileList(VideoFileList &list,int fromindex,int toindex);

    void showMp4FileList(QList<MP4FileInfo> &list);
    void showMp4FileList(QList<MP4FileInfo> &list,int fromindex,int toindex);

    void showFileInfoList(QFileInfoList &list);
    void showFileInfoList(QFileInfoList &list,int fromindex,int toindex);
    void showNormalFileInfoList(QFileInfoList &list,int fromindex,int toindex);
    void showNormalFileInfoList(QFileInfoList &list);
    void onShowSlot(QDir dir);
    void onDirShowSlot(QString &filename);
    Q_INVOKABLE void oncellDoubleClickedSlot(int row,int column);
    Q_INVOKABLE void onBackButtonClickedSlot();
    Q_INVOKABLE void searchFile(int type,int Chn,int filetype,QString starttime,QString endtime);
    Q_INVOKABLE void preViewFile(int Chn ,int filetype);
    Q_INVOKABLE int playVideoList(int type,int Chn,int filetype,QString starttime,QString endtime);
    Q_INVOKABLE void cancelProcess();


    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const; //重新实现roleNames()

    QStringList roles() const;
    void setRoles(const QStringList roles);
    QString name() const;
    QString pathname() const;
//    QQmlListProperty<QFileInfo> getFileInfoList() const;

private slots:
    void refrushModel();
private:
    void priSearchFile(int type,int Chn,int filetype,QString starttime,QString endtime);
    void priPreViewFile(int Chn ,int filetype);
    VideoFileList searchFile(VideoFileList &list,uint sttime,uint endtime);
    int search(QFileInfoList &list, int startindex, int endindex, uint time);
    int normalSearch(QFileInfoList &list,uint sttime,uint endtime,int flag);
    int getFrameIndex(QFileInfo &fileinfo,uint time);
    HI_S32 getAlarmFileName(int Chn, VIDEO_TYPE type, char *filename, int len);
    int findAlarmFile(int Chn, VIDEO_TYPE type, QFileInfoList &list);

//    static void appendGuest(QQmlListProperty<QFileInfo> *, QFileInfo *fileinfo);

signals:
    void filelistChangeSignal(VideoFileList &);
    void filelistChangeSignal(QList<MP4FileInfo> &);
//    void setWinNumSignal(int);
    void dataChanged();
    void fileNameChanged();
    void pathChanged();
    void refrushModelSignal();
private:
#ifndef LUNUX_WIN
    const QString RootPath = "/mnt/sda1/venc/";
#else
    const QString RootPath = "/home/abhw/venc/";
#endif
#ifndef LUNUX_WIN
    const char *ALARM_FILE_PATH = "/mnt/sda1/alarm";
#else
    const char *ALARM_FILE_PATH = "/home/abhw/nfsroot";
#endif
    const char *IO_FILE = ".io_alarm";
    const char *MOVED_FILE = ".move_alarm";

    QList<QVariantList> m_data;
    QStringList m_roleList;
    QString mFileName;
    QString mCurrentPath;
    QStack <QString> mPath;
    QFileInfoList mFileInfoList;
    VideoSearch mVideoSearch;
    VideoFileList mVideoFileList;
    QFuture<void> mProcess;
    bool mProcess_Run;
    QList<MP4FileInfo> mMp4FileList;


};

#endif // MYTABLEVIEWMODEL_H

