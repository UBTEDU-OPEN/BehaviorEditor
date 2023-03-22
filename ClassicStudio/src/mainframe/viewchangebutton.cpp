#include "viewchangebutton.h"
#include<QPixmap>
#include<QPainter>
#include<QDebug>
#include<QPoint>

ViewChangeButton::ViewChangeButton(QWidget *parent)
{

}

void ViewChangeButton::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    QPoint p = event->pos();
    int w = this->width();
    int h = this->height();
    QVector2D topLeft(0,0);
    QVector2D topRight(w,0);
    QVector2D bottomLeft(0,h);
    QVector2D bottomRight(w,h);
    QVector2D center(w/2,h/2);
    float x = event->x();
    float y = event->y();
    bool isInTop = pointInTriangle(topLeft,
                                   topRight,center,QVector2D(x,y));
    if(isInTop) {
        if(mDefaultDirection != TOP){
            mDefaultDirection = TOP;
            update();
            emit selectDirection(mDefaultDirection);
        }
        return;
    }
    bool isInRight = pointInTriangle(topRight,
                                   bottomRight,center,QVector2D(x,y));
    if(isInRight){
        if(mDefaultDirection != RIGHT){
            mDefaultDirection = RIGHT;
            update();
            emit selectDirection(mDefaultDirection);
        }
        return;
    }

    bool isInBottom = pointInTriangle(bottomLeft,
                                      bottomRight,center,QVector2D(x,y));
    if(isInBottom){
        if(mDefaultDirection != BOTTOM){
            mDefaultDirection = BOTTOM;
            update();
            emit selectDirection(mDefaultDirection);
        }
        return;
    }

    bool isInLeft = pointInTriangle(topLeft,
                                    bottomLeft,center,QVector2D(x,y));
    if(isInLeft){
        if(mDefaultDirection != LEFT){
            mDefaultDirection = LEFT;
            update();
            emit selectDirection(mDefaultDirection);
        }
        return;
    }
}

void ViewChangeButton::paintEvent(QPaintEvent *event)
{
    QPixmap m_pBg;
    m_pBg.load(":/image/res/image/virtualRobotBottom/ic_view.png");
    QPainter painter;
    painter.begin(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.translate(this->width() / 2, this->height() / 2);
    if(mDefaultDirection == TOP){
        painter.rotate(0);
    }else if(mDefaultDirection == RIGHT){
        painter.rotate(90);
    }else if(mDefaultDirection == BOTTOM){
        painter.rotate(180);
    }else if(mDefaultDirection == LEFT){
        painter.rotate(270);
    }
    painter.translate(-(this->width() / 2), -(this->height() / 2));
    painter.drawPixmap(rect(),m_pBg);
    painter.end();


}

bool ViewChangeButton::pointInTriangle(QVector2D A, QVector2D B, QVector2D C, QVector2D P)
{
    QVector2D v0 = C - A;
    QVector2D v1 = B - A;
    QVector2D v2 = P - A;
    float dot00 = QVector2D::dotProduct(v0, v0);
    float dot01 = QVector2D::dotProduct(v0, v1);
    float dot02 = QVector2D::dotProduct(v0, v2);
    float dot11 = QVector2D::dotProduct(v1, v1);
    float dot12 = QVector2D::dotProduct(v1, v2);
    float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * inverDeno ;
    if (u < 0 || u > 1)
    {
        return false;
    }
    float v = (dot00 * dot12 - dot01 * dot02) * inverDeno ;
    if (v < 0 || v > 1)
    {
        return false;
    }
    return u + v <= 1;

}

void ViewChangeButton::setDirection(Direction direction)
{
    mDefaultDirection = direction;
    update();
}
