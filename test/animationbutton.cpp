#include "animationbutton.h"
#include <QPainter>
#include <QDebug>
#include <QStyleOption>

AnimationButton::AnimationButton(QWidget *parent) : QWidget(parent)
{
    enter = false;
    leave = false;
    pixWidth = 0;
    pixHeight = 0;
    oldWidth = 0;
    oldHeight = 0;

//    this->setStyleSheet("background-color:black;");

    enterAnimation = new QPropertyAnimation(this, "");
    enterAnimation->setStartValue(0);
    enterAnimation->setEndValue(10);
    enterAnimation->setDuration(200);
    connect(enterAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(enterImageChanged(QVariant)));


    leaveAnimation = new QPropertyAnimation(this, "");
    leaveAnimation->setStartValue(0);
    leaveAnimation->setEndValue(10);
    leaveAnimation->setDuration(200);
    connect(leaveAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(leaveImageChanged(QVariant)));

}

void AnimationButton::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton){
        emit clicked();
        qDebug()<<targetWidth<<"  "<<targetHeight;
    }

}

void AnimationButton::enterEvent(QEvent *)

{

    enter = true;

    leave = false;

    pixWidth = pixWidth - 25;

    pixHeight = pixHeight - 25;

    enterAnimation->start();

}



void AnimationButton::leaveEvent(QEvent *)

{

    enter = false;

    leave = true;

    pixWidth = oldWidth;

    pixHeight = oldHeight;

    leaveAnimation->start();

}



void AnimationButton::paintEvent(QPaintEvent *event)

{
    Q_UNUSED(event);

    if (image.isEmpty()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setOpacity(0.2);

        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        painter.drawRect(rect());




    QPixmap pix(image);
    pix = pix.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

//    if (enter || leave) {
        if(enter)
            painter.setOpacity(1);
        int pixX = rect().center().x() - targetWidth / 2;
        int pixY = rect().center().y() - targetHeight / 2/* - 10*/;
        QPoint point(pixX, pixY);
        painter.drawPixmap(point, pix);
        painter.drawText(QRectF(0, height() - 20, width(), 20), Qt::AlignCenter, text);

//    }

}



void AnimationButton::enterImageChanged(QVariant index)

{
    int i = index.toInt();
    qDebug()<<"enter i="<<i;

    targetWidth = pixWidth + i * 5;
    targetHeight = pixHeight + i * 5;
    update();
}


void AnimationButton::leaveImageChanged(QVariant index)

{
    int i = index.toInt();
    qDebug()<<"i="<<i;

    targetWidth = pixWidth - i * 5;
    targetHeight = pixWidth - i * 5;
    update();

}



void AnimationButton::setImage(QString image)

{

    this->image = image;
    QPixmap pix(image);
    pixWidth = pix.width();
    pixHeight = pix.height();
    oldWidth = pixWidth;
    oldHeight = pixHeight;
    targetWidth = pixWidth - 25;
    targetHeight = pixHeight - 25;
    update();
}


void AnimationButton::setText(QString text)

{
    this->text = text;
    update();

}
