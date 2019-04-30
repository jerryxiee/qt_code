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

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void paintEvent(QPaintEvent *event);

protected:
    void mousePressEvent(QMouseEvent * event);
        void contextMenuEvent(QContextMenuEvent* e);

public slots:
        void changestatus(const QString &path);
private slots:
    void onMakefile();
    void ExitSlot();
private:
    const QString watchfile = "file";
    TestWidget *testwidget;
    QTimer *timer;
    QMenu *mainmenu;
    QAction *exit;
    QStackedWidget *mStackWidget;
    Ui::Widget *ui;
    QFileSystemWatcher * mFileWatch;
};

#endif // WIDGET_H
