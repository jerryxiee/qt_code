#ifndef SYSTEMLOG_H
#define SYSTEMLOG_H

#include <QObject>
#include <QFile>

class SystemLog : public QObject
{
    Q_OBJECT
public:
    explicit SystemLog(QObject *parent = nullptr,QString logname ="/opt/log/log");
    virtual ~SystemLog();

    void logwrite(int priority,QString str);
    void logwrite(int priority,const char *fmt,...);

    static SystemLog *getSystemLog();

signals:

public slots:

private:
//    const QString LOGPATHNAME = "/opt/log/log";
    const qint64 LOGMAXSIZE = 1024*1024*4;
    QFile mlogfile;

    static SystemLog *mSystemLog;
};

#endif // SYSTEMLOG_H
