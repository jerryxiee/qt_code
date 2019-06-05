#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "vencset.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    static VencSet *getVencIni();

signals:

public slots:

private:
    static VencSet *mVencIni;

};

#endif // SETTINGS_H
