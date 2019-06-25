#ifndef REGIONMANAGE_H
#define REGIONMANAGE_H

#include <QWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QRect>
#include "settings/settings.h"

class RegionManage : public QWidget
{
    Q_OBJECT
public:
    explicit RegionManage(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event);

protected:
    void contextMenuEvent(QContextMenuEvent* e);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);


signals:
    void exitSignal();
    void timePosChange(int,QPoint);
    void namePosChange(int,QPoint);
    void setRgnChange();

public slots:
    void onRegionSetSlot(int Chn, QString type);
    void onExitSlot();
    void onSetNameSlot();
    void onSetTimeSlot();
    void onOverlaySetPosSlot(int Chn);
    void onSetRgnChangedSlot();
    void onHidQmlSlot();

private:
    const QString SET_REGION = "region";
    DispSet *mDispSetIni;
    QMenu *mMenu;
    QAction *mExit;
    QAction *mSetName;
    QAction *mSetTime;
    QActionGroup *mActionGrp;
    bool mSetDsipName;
    bool mSetDispTime;
    QString mSetType;
    int mChn;
    QPoint mStartPoint;
    QPoint mEndPoint;
    QPoint mTempPoint;
    QRect mTimeRect;
    QRect mNameRect;
    bool mMove;
    bool mChange;
};

#endif // REGIONMANAGE_H
