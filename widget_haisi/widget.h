#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtQuickWidgets>
#include <QEvent>
#include <QMouseEvent>

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

private slots:
    //void on_pushButton_clicked();

private:
    bool load_qml;
    QQuickWidget* m_quickWidget;
    QQuickView * m_quckView;
    Ui::Widget *ui;
    QQmlApplicationEngine engine;
};

#endif // WIDGET_H
