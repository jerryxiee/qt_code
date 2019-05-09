#ifndef PLAYVIDEO_H
#define PLAYVIDEO_H

#include <QWidget>
#include <QtQuickWidgets>
#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QFileSystemModel>
#include <QTreeView>
#ifdef arm
#include <video/vdec.h>
#endif

class PlayVideo : public QWidget
{
    Q_OBJECT
public:
    explicit PlayVideo(QWidget *parent = nullptr);
    ~PlayVideo();

//    void paintEvent(QPaintEvent *event);
    void showFileInfoList(QFileInfoList list);

signals:
    void VideoDispSignal(QString &filename);

public slots:
    void onStartVdecSlot();
    void onFindButtonClickedSlot();
    void onWatchdButtonClickedSlot();
    void onOpenButtonClickedSlot();
    void onCloseButtonClickedSlot();
    void oncontextMenuSlot(const QPoint &pos);
    void onShowSlot(QDir dir);
    void onDirShowSlot(QString &filename);
    void oncellDoubleClickedSlot(int row,int column);
    void onBackButtonClickedSlot();


protected:
//    void mousePressEvent(QMouseEvent * event);
    void contextMenuEvent(QContextMenuEvent* e);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
#ifdef arm
    Vdec mVdec;
#endif
    enum { absoluteFileNameRole = Qt::UserRole + 1 };

    QStack <QString> mPath;
    QTreeView *mTreeView;
    QFileSystemModel *mFileModel;
    QTableWidget *mFileTabWidget;
    QQuickWidget* mQuickWidget = nullptr;
    QString mCurrentPath;
    QPushButton *mButton;
    QLabel *mMainLab;
    QPushButton *mback;
    bool mVdecStart;

};

#endif // PLAYVIDEO_H
