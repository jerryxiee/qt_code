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

signals:
    void exitSignal();

public slots:
    void onRegionSetSlot(int Chn, QString type);
    void onExitSlot();
    void onSetNameSlot();
    void onSetTimeSlot();

private:
    const QString SET_REGION = "region";
    DispSet *mDispSetIni;
    QMenu *mMenu;
    QAction *mExit;
    QAction *mSetName;
    QAction *mSetTime;
    bool mSetDsipName;
    bool mSetDispTime;
    QString mSetType;
    int mChn;
    QPoint mStartPoint;
    QPoint mEndPoint;
};

#endif // REGIONMANAGE_H
