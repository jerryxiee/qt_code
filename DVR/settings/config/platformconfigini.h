#ifndef PLATFORMCONFIGINI_H
#define PLATFORMCONFIGINI_H

#include <QObject>
#include "../config.h"
#include "deviceparamconfig.h"

class PlatformConfigIni : public QObject,public Config
{
    Q_OBJECT
public:
    explicit PlatformConfigIni(QObject *parent = nullptr, QString filename = ".platform.ini");

    bool insertSignalData(DeviceParam &param);
    QVariant getValue(int id,int num,int field) const;


signals:

public slots:

private:
    const QString ROOTNAME = "platform";
};

#endif // PLATFORMCONFIGINI_H
