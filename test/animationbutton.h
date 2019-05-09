#ifndef ANIMATIONBUTTON_H
#define ANIMATIONBUTTON_H

#include <QWidget>
#include <QVariant>
#include <QPropertyAnimation>
#include <QMouseEvent>

class AnimationButton : public QWidget
{
    Q_OBJECT
public:
    explicit AnimationButton(QWidget *parent = nullptr);

signals:
    void clicked();
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent * event);



public slots:

    //设置显示的文字
    void setText(QString text);

    //设置显示的图像
    void setImage(QString image);

private slots:
    void enterImageChanged(QVariant index);

    void leaveImageChanged(QVariant index);


private:
    bool enter;                         //是否进入
    bool leave;                         //是否离开
    int pixWidth;                       //图片宽度
    int pixHeight;                      //图片高度
    int oldWidth;                       //图片旧宽度
    int oldHeight;                      //图片旧高度
    QPropertyAnimation *enterAnimation; //进入动画
    QPropertyAnimation *leaveAnimation; //离开动画
    int targetWidth;                    //目标宽度
    int targetHeight;                   //目标高度
    QString text;                       //显示文字
    QString image;                      //图像路径

};

#endif // ANIMATIONBUTTON_H
