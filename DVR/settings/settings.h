#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "vencset.h"
#include "dispset.h"
#include "systemset.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    static VencSet *getVencIni();
    static DispSet *getDispSetIni();
    static SystemSet *getSystemSetIni();

signals:

public slots:

private:
    static VencSet *mVencIni;
    static DispSet *mDispSetIni;
    static SystemSet *mSystemSetIni;

};

#endif // SETTINGS_H
