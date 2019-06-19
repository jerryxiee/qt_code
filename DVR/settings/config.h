#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QVariant>

class Config
{
public:
    Config();
    Config(const QString &filename);
    virtual ~Config();

    void setFileName(const QString &filename);
    void setConfig(const QString &rootName,const QString &name,const QVariant &Value);
    QVariant getConfig(const QString &rootName,const QString &name) const;



private:
#ifndef LUNUX_WIN
    const QString CONFIG_PATH = "/usr/dvr_config/";
#else
    const QString CONFIG_PATH = "/home/abhw/nfsroot/dvr_config/";
#endif
    QString mFileName;
    QSettings *mpSetting;
};

#endif // CONFIG_H
