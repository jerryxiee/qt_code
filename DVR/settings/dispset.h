#ifndef DISPSET_H
#define DISPSET_H

#include <QObject>
#include <QString>
#include <QVariant>
#include "config.h"

class DispSet : public QObject ,public Config
{
    Q_OBJECT
//    Q_PROPERTY(bool entime WRITE enableTime /*NOTIFY onTimeChanged*/)
//    Q_PROPERTY(bool enweek WRITE enableWeek /*NOTIFY onWeekChanged*/)
//    Q_PROPERTY(bool enname WRITE enableName /*NOTIFY onNameChanged*/)
public:
    explicit DispSet(QObject *parent = nullptr,const QString &filename = ".dispset.ini");

//    void setConfig(const QString name,const QVariant Value);
//    QVariant getConfig(const QString name) const;

    Q_INVOKABLE void  setConfig(int Chn, QString name, bool dispname, bool dispdate,
                                bool dispweek, QString datetype, int value);
    void setConfig(const QString &rootName,const QString &name,const QVariant &Value);

    Q_INVOKABLE void  preSetConfig(int Chn, QString name, bool dispname, bool dispdate,
                                bool dispweek, QString datetype, int value);

    Q_INVOKABLE void enableTime(int Chn, bool enable);
    Q_INVOKABLE void enableWeek(int Chn, bool enable);
    Q_INVOKABLE void enableName(int Chn, bool enable);

    void setNameRect(int Chn,QRect &rect);
    QRect &getNameRect(int Chn,QRect &rect);
    void setTimeRect(int Chn,QRect &rect);
    QRect &getTimeRect(int Chn, QRect &rect);
    void setTimeShowEnabled(int Chn,bool enable);
    bool getTimeShowEnabled(int Chn);
    void setNameShowEnabled(int Chn,bool enable);
    bool getNameShowEnabled(int Chn);
    void setWeekShowEnabled(int Chn,bool enable);
    bool getWeekShowEnabled(int Chn);
    void setTimeType(int Chn,QString &type);
    QString &getTimeType(int Chn, QString &type);
    void setDateType(int Chn,int type);
    int getDateType(int Chn);

signals:
    void overlayTimeTypeChange(QString);
    void overlayNameChange(int, QString);
    void overlaySetPos(int);
    void rgnOverlayShow(int);

public slots:
//    void resumeset();


public:
    const QString RootName = "Channel";
    const QString ChnName = "Name";
    const QString DateType = "DateType";
    const QString TimeType = "TimeType";
    const QString DispName = "DispName";
    const QString DispDate = "DispDate";
    const QString DispWeek = "DispWeek";
    const QString NamePos = "NamePos";
    const QString TimePos = "TimePos";

private:
#ifndef LUNUX_WIN
    const QString DISPSET_CONFIG_PATH = "/usr/dvr_config/.dispset.ini";
#else
    const QString DISPSET_CONFIG_PATH = "/home/abhw/nfsroot/dvr_config/.dispset.ini";
#endif
};

#endif // DISPSET_H
