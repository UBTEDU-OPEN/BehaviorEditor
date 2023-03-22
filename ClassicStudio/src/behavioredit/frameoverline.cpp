#include "frameoverline.h"

#include <QPainter>
#include <QBrush>

FrameOverLine::FrameOverLine(QWidget *parent) : QWidget(parent),frameIndex_(0)
{
}

void FrameOverLine::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.save();
    painter.fillRect(e->rect(), QBrush(Qt::red));
    painter.restore();
}

void FrameOverLine::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons().testFlag(Qt::LeftButton)) {
        emit sigDragFrameOverLine(e->globalPos());
    }
}
