#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtQuickWidgets>

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

private slots:
    void on_pushButton_clicked();

private:
    QQuickWidget* m_quickWidget;
    Ui::Widget *ui;
};

#endif // WIDGET_H
