#include "regioncontrol.h"
#include <QDebug>

RegionControl* RegionControl::mRegionCtr = nullptr;

RegionControl* RegionControl::createNew()
{
    if(!mRegionCtr){
        mRegionCtr = new RegionControl();
    }

    return mRegionCtr;
}

RegionControl::RegionControl(QObject *parent) : QObject(parent)
{
    RectangleRegionDataBase rectangledatabase;
    CircularRegionDataBase circulardatabase;
    RectangleRegionAttr rectattr;
    CicularRegionAttr circularattr;
    RegionBase *region;
    int num = 0;

    if(rectangledatabase.isExists()){
        num = rectangledatabase.count();
        for (int i = 0 ;i < num;i ++) {
            if(rectangledatabase.getSignalData(i+1,rectattr)){
                if(lockup(REGIONTYPE::Rectange,rectattr.regionId)){

                    continue;
                }
                region = new  RectangleRegion(rectattr);
                if(region){
                    mRegionHashTab.insert(genreatename(region->getRegionType(),region->getRegionId()),region);
                }
            }
        }
        qDebug()<<"rectangleregion num:"<<num;
    }

    if(circulardatabase.isExists()){
        num = circulardatabase.count();
        for (int i = 0 ;i < num;i ++) {
            if(circulardatabase.getSignalData(i+1,circularattr)){
                if(lockup(REGIONTYPE::Circular,circularattr.regionId)){

                    continue;
                }
                region = new  CircularRegion(circularattr);
                if(region){
                    mRegionHashTab.insert(genreatename(region->getRegionType(),region->getRegionId()),region);
                }
            }
        }
        qDebug()<<"circularregion num:"<<num;
    }

}

RegionControl::~RegionControl()
{

}

QString RegionControl::genreatename(REGIONTYPE type,uint id)
{
    QString name;
    switch (type) {
        case REGIONTYPE::Circular:
        name = "C"+QString::number(id);
        break;
        case REGIONTYPE::Rectange:
        name = "R"+QString::number(id);
        break;
        case REGIONTYPE::Other:
        name = "O"+QString::number(id);
        break;
    }

    return name;
}
RegionBase *RegionControl::lockup(QString key)
{
    QHashIterator<QString,RegionBase *> hashtab(mRegionHashTab);
    while (hashtab.hasNext()) {
        hashtab.next();
        if(hashtab.key() == key){
            return hashtab.value();
        }
    }
    return nullptr;
}

RegionBase *RegionControl::lockup(REGIONTYPE type,uint id)
{
    QString key = genreatename(type,id);

    return lockup(key);
}



bool RegionControl::addRegion(RegionBase *region)
{
    if(!region){
        return false;
    }
    QString key = genreatename(region->getRegionType(),region->getRegionId());
    if(lockup(key)){
        return false;
    }

    region->addRegionToDataBase();
    mRegionHashTab.insert(key,region);
    connect(region,SIGNAL(enterRegion(REGIONTYPE ,uint32_t,bool)),this,SLOT(onEnterRegionSlot(REGIONTYPE ,uint32_t,bool)));
//    mRegionHashTab.insert(region->getRegionId(),region);
    qDebug()<<"addRegion end";
    return true;
}

void RegionControl::deleteRegion(REGIONTYPE type,uint id)
{
    RegionBase * region = lockup(type,id);
    if(!region){
        qDebug()<<"not in hashtab";
        return;
    }
    disconnect(region,SIGNAL(enterRegion(REGIONTYPE ,uint32_t,bool)),this,SLOT(onEnterRegionSlot(REGIONTYPE ,uint32_t,bool)));
    region->deleteRegionFromDataBase();
    mRegionHashTab.remove(genreatename(type,id));
    delete region;

    qDebug()<<"deleteregion end";

}

void RegionControl::onEnterRegionSlot(REGIONTYPE type,uint32_t id,bool enter)
{

}
