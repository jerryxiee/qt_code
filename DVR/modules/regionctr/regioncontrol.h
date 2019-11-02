#ifndef REGIONCONTROL_H
#define REGIONCONTROL_H

#include <QObject>
#include <QHash>
#include <QList>
#include "rectangleregion.h"
#include "circularregion.h"

class RegionControl : public QObject
{
    Q_OBJECT
public:
    static RegionControl* getRegionControlObject();

    RegionBase *lockup(REGIONTYPE type,uint id);
    RegionBase *lockup(QString key);
    bool addRegion(RegionBase *region);
    void deleteRegion(REGIONTYPE type,uint id);
    void runRegionFunction(uint32_t latitude,uint32_t longitude);
    ~RegionControl();


protected:
    explicit RegionControl(QObject *parent = nullptr);
private:
    QString genreatename(REGIONTYPE type,uint id);
signals:
    void enterOrExitRegionSignal(REGIONTYPE type,uint32_t id,bool enter);
    void reportToPlatformSignal(REGIONTYPE type,uint32_t id,bool enter);
    void reportToDriverSignal(REGIONTYPE type,uint32_t id,bool enter);
    void overSpeedSignal(REGIONTYPE type,uint32_t id);

public slots:
    void onEnterRegionSlot(REGIONTYPE type,uint32_t id,bool enter,uint32_t attr);
    void onOverSpeedSlot(REGIONTYPE type,uint32_t id);

private:
    static RegionControl* mRegionCtr;
    QHash<QString,RegionBase *> mRegionHashTab;
//    QHash<uint,RegionBase *> mRectangleHashTab;
};

#endif // REGIONCONTROL_H
