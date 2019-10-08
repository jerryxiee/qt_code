#ifndef DEVICEPARAMCONFIG_H
#define DEVICEPARAMCONFIG_H

#include <QObject>
#include <QList>
#include "common/sqlitedatebase.h"

typedef struct{
    int id;
    int num;
    int field;
    QString value;
} DeviceParam;

typedef QList<DeviceParam> DeviceParamList;

class DeviceParamConfig : public QObject,public SqliteDateBase
{
    Q_OBJECT

public:
    static DeviceParamConfig *getDeviceConfig();

    bool createTable();
    bool isTabExists();
    bool insertSignalData(DeviceParam &param);
    bool insertMorelData(DeviceParamList &paramlist);
    bool updateData(int id,int num,int field,QString value);
    QString getValue(int id,int num,int field);
    QStringList getValue(int id,int num);
private:
    explicit DeviceParamConfig(QObject *parent = nullptr);

signals:

public slots:


private:
    const QString TABNAME = "deviceparamtab";

    static DeviceParamConfig *mDeviceConfig;
};

#endif // DEVICEPARAMCONFIG_H
