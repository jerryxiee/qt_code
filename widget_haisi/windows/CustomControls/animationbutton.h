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
    explicit AnimationButton(QString &text,QWidget *parent = nullptr);

private:
    void Init();
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
    void setImageSize(QSize &size);
    void setImageSize(int width,int height);
    void setOpacity(qreal opacity);
    void setDuration(int msecs);
    void setValue(int start,int end);

private slots:
    void onEnterImageChangedSlot(QVariant index);

    void onLeaveImageChangedSlot(QVariant index);

public:
    int mStartValue;
    int mEndValue;
    int mDuration;
private:
    bool mEnter;                         //是否进入
    int mpixWidth;                       //图片宽度
    int mpixHeight;                      //图片高度
    int moldWidth;                       //图片旧宽度
    int moldHeight;                      //图片旧高度
    int mfontWidth;
    int mfontHeight;
    int menterIndex;
    qreal msticopacity;
    QPropertyAnimation *enterAnimation; //进入动画
    QPropertyAnimation *leaveAnimation; //离开动画
    int mtargetWidth;                    //目标宽度
    int mtargetHeight;                   //目标高度
    QString mtext;                       //显示文字
    QString mimage;                      //图像路径
};

#endif // ANIMATIONBUTTON_H
