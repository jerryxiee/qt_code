#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QEvent>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <QStackedWidget>
#include <QMenu>
#include "testwidget.h"
#include <QFileSystemWatcher>
#include <QFileSystemModel>
#include <QTreeView>
#include <QStandardItemModel>
#include <QFileInfoList>

#include <QListWidgetItem>
#include <QDir>
#include <QLineEdit>
#include <QVBoxLayout>
#include "animationbutton.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void showFileInfoList(QFileInfoList list);

    void paintEvent(QPaintEvent *event);

    Q_INVOKABLE void test(QString str);


protected:
    void mousePressEvent(QMouseEvent * event);
        void contextMenuEvent(QContextMenuEvent* e);

public slots:
        void changestatus(const QString &path);
        void slotShow(QDir dir);
        void slotDirShow(QListWidgetItem *item);
        void onHidQmlSlot();

private slots:
    void onMakefile();
    void ExitSlot();
    void onClickedSlot();
private:
    QQuickWidget* m_quickWidget;
    const QString watchfile = "file";
    TestWidget *testwidget;
    QTimer *timer;
    QMenu *mainmenu;
    QAction *exit;
    QStackedWidget *mStackWidget;
    Ui::Widget *ui;
    QFileSystemWatcher * mFileWatch;
    QFileSystemModel *mFileModel;
    QTreeView *mTreeView;

    QLineEdit *fileLineEdit;
    QListWidget *fileListWidget;
    QVBoxLayout *mainLayout;
};

#endif // WIDGET_H
