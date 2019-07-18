#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QFileInfoList>
#include <QDir>
#include <QStack>
#include <QQmlListProperty>
#include "common/sample_comm.h"

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

    void showFileInfoList(QFileInfoList &list);
    void showFileInfoList(QFileInfoList &list,int fromindex,int toindex);
    void onShowSlot(QDir dir);
    void onDirShowSlot(QString &filename);
    Q_INVOKABLE void oncellDoubleClickedSlot(int row,int column);
    Q_INVOKABLE void onBackButtonClickedSlot();
    Q_INVOKABLE bool searchFile(int type,int Chn,int filetype,QString starttime,QString endtime);
    Q_INVOKABLE void preViewFile();
    Q_INVOKABLE int playVideoList(int type,int Chn,int filetype,QString starttime,QString endtime);


    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const; //重新实现roleNames()

    QStringList roles() const;
    void setRoles(const QStringList roles);
    void refrushModel();
    QString name() const;
    QString pathname() const;
//    QQmlListProperty<QFileInfo> getFileInfoList() const;

private:
    int search(QFileInfoList &list, int startindex, int endindex, uint time);
    int normalSearch(QFileInfoList &list,uint sttime,uint endtime,int flag);
    HI_S32 getAlarmFileName(int Chn, VIDEO_TYPE type, char *filename, int len);
    int findAlarmFile(int Chn, VIDEO_TYPE type, QFileInfoList &list);

//    static void appendGuest(QQmlListProperty<QFileInfo> *, QFileInfo *fileinfo);

signals:
    void filelistChangeSignal(QFileInfoList &);
    void dataChanged();
    void fileNameChanged();
    void pathChanged();
private:
#ifndef LUNUX_WIN
    const QString RootPath = "/mnt/sda1/venc/";
#else
    const QString RootPath = "/home/abhw/venc/";
#endif
#ifndef LUNUX_WIN
    const char *ALARM_FILE_PATH = "/opt/alarm";
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

};

#endif // MYTABLEVIEWMODEL_H

