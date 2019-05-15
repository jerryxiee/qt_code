#include "animationbutton.h"
#include <QDebug>
#include <QPainter>

AnimationButton::AnimationButton(QWidget *parent) : QWidget(parent)
{

    Init();
}

AnimationButton::AnimationButton(QString &text,QWidget *parent) : QWidget(parent)
{
    this->mtext = text;
    Init();
}

void AnimationButton::Init()
{
    mEnter = false;
    mpixWidth = 0;
    mpixHeight = 0;
    moldWidth = 0;
    moldHeight = 0;
    msticopacity = 1;
    mStartValue = 0;
    mEndValue = 0;
    mDuration = 0;
    menterIndex = -1;

    enterAnimation = new QPropertyAnimation(this, "");
//    enterAnimation->setStartValue(0);
//    enterAnimation->setEndValue(5);
//    enterAnimation->setDuration(200);
    connect(enterAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(onEnterImageChangedSlot(QVariant)));

    leaveAnimation = new QPropertyAnimation(this, "");
//    leaveAnimation->setStartValue(0);
//    leaveAnimation->setEndValue(5);
//    leaveAnimation->setDuration(200);
    connect(leaveAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(onLeaveImageChangedSlot(QVariant)));
}

void AnimationButton::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton){
        emit clicked();
//        qDebug()<<mtargetWidth<<"  "<<mtargetHeight;
    }

}

void AnimationButton::enterEvent(QEvent *)

{

    mEnter = true;

//    mpixWidth = mpixWidth;

//    mpixHeight = mpixHeight;
    if(mDuration > 0){
        enterAnimation->setStartValue(mStartValue);
        enterAnimation->setEndValue(mEndValue);
        enterAnimation->setDuration(mDuration);
        enterAnimation->start();
    }

}



void AnimationButton::leaveEvent(QEvent *)

{
    mEnter = false;
    mpixWidth = moldWidth;

    if(mDuration > 0){
        mpixHeight = moldHeight;
        leaveAnimation->setStartValue(mStartValue);
        leaveAnimation->setEndValue(mEndValue);
        leaveAnimation->setDuration(mDuration);
        leaveAnimation->start();
    }

}



void AnimationButton::paintEvent(QPaintEvent *event)

{
    Q_UNUSED(event);

//    if (image.isEmpty()) {
//        return;
//    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


//        painter.setPen(Qt::NoPen);
//        painter.setBrush(Qt::black);
//        painter.drawRect(rect());

    if (!mimage.isEmpty()) {
        QPixmap pix(mimage);
        pix = pix.scaled(mtargetWidth, mtargetHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.setOpacity(msticopacity);
        if(mEnter)
            painter.setOpacity(1);
        int pixX = rect().center().x() - mtargetWidth / 2;
        int pixY = rect().center().y() - mtargetHeight / 2/* - 10*/;
        QPoint point(pixX, pixY);
        painter.drawPixmap(point, pix);
    }
    painter.drawText(QRectF((width() - mfontWidth)/2, (height() - mfontHeight)/2, mfontWidth, mfontHeight), Qt::AlignCenter, mtext);

}

void AnimationButton::onEnterImageChangedSlot(QVariant index)
{
    int i = index.toInt();
//    qDebug()<<"mtargetWidth="<<mtargetWidth<<"menterIndex"<<menterIndex;
    mtargetWidth += i;
    if(mtargetWidth > width()){
        mtargetWidth = width();
    }
    mtargetHeight += i;
    if(mtargetHeight > height()){
        mtargetHeight = height();
    }
    update();
}

void AnimationButton::onLeaveImageChangedSlot(QVariant index)

{
    int i = index.toInt();
//    qDebug()<<"mtargetWidth="<<mtargetWidth<<"menterIndex"<<menterIndex;

    mtargetWidth -= i;
    if(mtargetWidth < mpixWidth){
        mtargetWidth = mpixWidth;
    }
    mtargetHeight -= i;
    if(mtargetHeight < mpixHeight){
        mtargetHeight = mpixHeight;
    }


    update();

}



void AnimationButton::setImage(QString image)

{

    this->mimage = image;
    QPixmap pix(image);
    mpixWidth = pix.width();
    mpixHeight = pix.height();
    moldWidth = mpixWidth;
    moldHeight = mpixHeight;
    mtargetWidth = mpixWidth;
    mtargetHeight = mpixHeight;
    update();
}

void AnimationButton::setText(QString text)
{
    QFontMetrics metric(this->font());

    this->mtext = text;
    mfontWidth = metric.width(text);
    mfontHeight = metric.height();
    update();

}

void AnimationButton::setImageSize(QSize &size)
{
    mpixWidth = size.width();
    mpixHeight = size.height();
    moldWidth = mpixWidth;
    moldHeight = mpixHeight;
    mtargetWidth = mpixWidth;
    mtargetHeight = mpixHeight;
}

void AnimationButton::setImageSize(int width,int height)
{
    mpixWidth = width;
    mpixHeight = height;
    moldWidth = mpixWidth;
    moldHeight = mpixHeight;
    mtargetWidth = mpixWidth;
    mtargetHeight = mpixHeight;
}
void AnimationButton::setOpacity(qreal opacity)
{
    msticopacity = opacity;
}

void AnimationButton::setDuration(int msecs)
{
    mDuration = msecs;
}
void AnimationButton::setValue(int startVal,int endVal)
{
    mStartValue = startVal;
    mEndValue = endVal;
}

