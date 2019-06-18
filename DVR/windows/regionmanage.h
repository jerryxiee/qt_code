#ifndef REGIONMANAGE_H
#define REGIONMANAGE_H

#include <QWidget>
#include <QMenu>
#include <QMouseEvent>

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

private:
    QMenu *mMenu;
    QAction *mExit;
};

#endif // REGIONMANAGE_H
