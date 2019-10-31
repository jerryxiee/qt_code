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
    static RegionControl* createNew();

    RegionBase *lockup(REGIONTYPE type,uint id);
    RegionBase *lockup(QString key);
    bool addRegion(RegionBase *region);
    void deleteRegion(REGIONTYPE type,uint id);
    ~RegionControl();


protected:
    explicit RegionControl(QObject *parent = nullptr);
private:
    QString genreatename(REGIONTYPE type,uint id);
signals:

public slots:
    void onEnterRegionSlot(REGIONTYPE type,uint32_t id,bool enter);

private:
    static RegionControl* mRegionCtr;
    QHash<QString,RegionBase *> mRegionHashTab;
//    QHash<uint,RegionBase *> mRectangleHashTab;
};

#endif // REGIONCONTROL_H
