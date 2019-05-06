#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtQuickWidgets>
#include <QEvent>
#include <QMouseEvent>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

public slots:

protected:
//    void mousePressEvent(QMouseEvent * event);
    void contextMenuEvent(QContextMenuEvent* e);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    QQuickWidget* mQuickWidget;
};

#endif // MAINWINDOW_H
