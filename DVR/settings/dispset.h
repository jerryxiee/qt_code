#ifndef DISPSET_H
#define DISPSET_H

#include <QObject>
#include <QString>
#include <QVariant>
#include "config.h"

class DispSet : public QObject ,public Config
{
    Q_OBJECT
public:
    explicit DispSet(QObject *parent = nullptr,const QString &filename = ".dispset.ini");

//    void setConfig(const QString name,const QVariant Value);
//    QVariant getConfig(const QString name) const;

    Q_INVOKABLE void  setConfig(int Chn, QString name, bool dispname, bool dispdate,
                                bool dispweek, QString datetype, int value);

signals:

public slots:


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
