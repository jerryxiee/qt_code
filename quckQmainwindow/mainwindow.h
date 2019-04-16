#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQuickWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

    QQuickWidget* m_quickWidget;


private slots:
    void on_pushButton_clicked();

private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
