#ifndef RAGIONBASE_H
#define RAGIONBASE_H

#include <QObject>
#include <QDateTime>

typedef enum{
    Circular,
    Rectange,
    Other
} REGIONTYPE;

Q_DECLARE_METATYPE(REGIONTYPE)

class RegionBase : public QObject
{
    Q_OBJECT
public:
    explicit RegionBase(REGIONTYPE type,QObject *parent = nullptr);
    RegionBase(REGIONTYPE type,uint32_t id,uint32_t attr,uint start,uint end,uint topspeed,uint topspeeddur,QObject *parent = nullptr);
//    virtual bool modifyRegion() = 0;
//    virtual bool updateRegion() = 0;
    virtual bool isRectangle() const;
    virtual bool isCircular() const;
    virtual bool addRegionToDataBase() = 0;
    virtual bool deleteRegionFromDataBase() = 0;
    virtual bool contaion(uint32_t latitude,uint32_t longitude) = 0;
    uint32_t getRegionId() const {return mRegionId;}
    void setRegionId(uint32_t id);
    uint32_t getRegionAttr() const {return  mRegionAttr;}
    void setRegionAttr(uint32_t value);
//    uint getStartTime() const {return mStartTime;}
//    void setStartTime(uint value);
//    uint getEndTime() const {return  mEndTime;}
//    void setEndTime(uint value);

    QDateTime getStartTime() const ;
    void setStartTime(QDateTime value);
    QDateTime getEndTime() const;
    void setEndTime(QDateTime value);

    uint getTopSpeed() const {return  mTopSpeed;}
    void setTopSpeed(uint value);
    uint getOverSpeedDuration() const {return mOverSpeedDuration;}
    void setOverSpeedDuration(uint value);
    void repeatCalcFunc(uint32_t latitude,uint32_t longitude);
    bool isEveryDay() const {return mRepeatEveryDay;}
    void setEveryDayEffect(bool enable);
    REGIONTYPE getRegionType() const {return mType;}
    virtual ~RegionBase();
    //n1:第一点的纬度坐标 e1:第一点的经度坐标
    static double distance(uint32_t n1, uint32_t e1, uint32_t n2, uint32_t e2);

signals:
    void enterRegion(REGIONTYPE type,uint32_t id,bool enter);

public slots:

private:
    uint32_t mRegionId;
    uint32_t mRegionAttr;
    uint mStartTime;
    uint mEndTime;
    uint mTopSpeed;
    uint mOverSpeedDuration;
    bool mIsInRegion;
    bool mRepeatEveryDay;
    QDateTime mStart;
    QDateTime mEnd;
    REGIONTYPE mType;
};

#endif // RAGIONBASE_H
