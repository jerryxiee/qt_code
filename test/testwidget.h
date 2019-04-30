#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QEvent>
#include <QMenu>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuickWidgets>

class TestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TestWidget(QWidget *parent = nullptr);
    ~TestWidget();

signals:

protected:
    void contextMenuEvent(QContextMenuEvent* e);

public slots:
    void ExitSlot();

private:
    QQuickWidget* m_quickWidget;
    QMenu *mainmenu;
    QAction *exit;
    QPushButton *button;
    QQmlApplicationEngine engine;
};

#endif // TESTWIDGET_H
