#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QRect>
#include <QBrush>
#include <QFont>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1280,720);

    m_quickWidget = new QQuickWidget();
    m_quickWidget = new QQuickWidget(this);//this基类为QWidget
    m_quickWidget->move(0,0);
    m_quickWidget->resize(400,400);
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    QUrl source("qrc:/main.qml");
    m_quickWidget->setSource(source);
    //m_quickWidget->deleteLater();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);// 创建QPainter一个对象

    painter.setCompositionMode( QPainter::CompositionMode_Clear );
    painter.fillRect( 10, 10, 500, 500, Qt::SolidPattern );

    /*// 画一条直线
    QPen pen;
    pen.setColor(Qt::yellow);           // 设置画笔为黄色
    painter.setPen(pen);                // 设置画笔
    painter.drawLine(rect().topLeft(), rect().bottomRight());
    // 画一个空心矩形
    pen.setColor(Qt::darkRed);
    painter.setPen(pen);
    painter.drawRect(QRect(1, 1, 100, 100));
    // 画一个实心矩形
    QBrush bruch(Qt::FDiagPattern);     // 画刷
    painter.setBrush(bruch);            // 设置画刷
    painter.drawRect(QRect(105, 1, 100, 100));
    // 画一个多点线
    pen.setColor(Qt::white);
    painter.setPen(pen);
    bruch.setStyle(Qt::NoBrush);
    // 将画刷设置成null
    painter.setBrush(bruch);
    static const QPointF points[4] = {QPointF(210.0, 1), QPointF(220.0, 50.3), QPointF(300, 100.4), QPointF(260.4, 120.0)};
    painter.drawPolyline(points, 4);
    // 画多个点
    QPointF pointf[10];
    for (int i=0; i<10; ++i)
    {
        pointf[i].setX(2.0+i*10.0);
        pointf[i].setY(130.0);
    }
    painter.drawPoints(pointf, 10);
    // 画多条线
    QLineF linef[5];
    for (int j=0; j<5; ++j)
    {
        linef[j].setP1(QPointF(110.9+j*10, 120.0));
        linef[j].setP2(QPointF(120.8+j*12, 200.0));
    }    painter.drawLines(linef, 5);

    // 画一个圆角矩形

    QRectF rectangle(290.0, 110.0, 50, 50);
    bruch.setStyle(Qt::SolidPattern);
    painter.setBrush(bruch);
    painter.drawRoundedRect(rectangle, 20.0, 15.0);
    // 画一个QString
    painter.drawText(50, 300, "Hello DevDiv!");
    //ps：下面这两个有点绿啊。。。。。。如果挡住了上面的图形的话就把他们两个注释掉吧
    //画一个椭圆
    painter.setRenderHint(QPainter::Antialiasing, true);//启用反走样，告诉QPainter用不同颜色强度绘制边框以减少视觉扭曲，这种扭曲一般
    //会在边框转换为像素的时候发生。由此生成的结果是的到一条平滑的曲线
    painter.setPen(QPen(Qt::black, 12, Qt::DashDotDotLine, Qt::RoundCap));
    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    painter.drawEllipse(80, 80, 400, 240);
    //绘制三次贝塞尔曲线
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;    path.moveTo(80, 320);
    path.cubicTo(200, 80, 320, 80, 480, 320);
    painter.setPen(QPen(Qt::black, 8));
    painter.drawPath(path);*/
}


void MainWindow::on_pushButton_clicked()
{
    m_quickWidget->deleteLater();
}
