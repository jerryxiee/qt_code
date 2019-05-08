#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//    void paintEvent(QPaintEvent *event);
    void showFileInfoList(QFileInfoList list);

signals:
    void VideoDispSignal(QString &filename);

public slots:
    void onStartVdecSlot();
    void onFindButtonClick();
    void onWatchdButtonClick();
    void onOpenButtonClick();
    void onCloseButtonClick();
    void contextMenu(const QPoint &pos);
    void slotShow(QDir dir);
    void slotDirShow(QString &filename);
    void testSlot(int row,int column);
    void onBackButtonClick();


protected:
//    void mousePressEvent(QMouseEvent * event);
    void contextMenuEvent(QContextMenuEvent* e);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
#ifdef arm
    Vdec mVdec;
#endif
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

#endif // MAINWINDOW_H
