#include "framepointer.h"

#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QMouseEvent>

const QRect FramePointer::dragArea((FramePointer::WIDTH - 13) / 2, FramePointer::LABEL_HEIGHT, 13, 15);

FramePointer::FramePointer(QWidget *parent) :
    QWidget(parent)
{
    setVisible(false);
    setFixedWidth(WIDTH);
    setFixedHeight(LABEL_HEIGHT+15);
}

FramePointer::~FramePointer()
{

}

void FramePointer::setFrameIndex(int fi)
{
    frameIndex_ = fi;
}

void FramePointer::setFrameRate(int fr)
{
    frameRate_ = fr;
}

QRect FramePointer::getDragArea() const
{
    return dragArea;
}

void FramePointer::paintEvent(QPaintEvent *)
{
    QRect paintRect = rect();
    paintRect.setHeight(LABEL_HEIGHT);
    QPainter painter(this);
    painter.save();
    QPen pen = painter.pen();
    pen.setColor(QColor(0x0,0xa5,0xff));
    painter.setPen(pen);
    painter.drawText(paintRect, QString::number(frameIndex_));

    paintRect = dragArea;
    QPainterPath paintPath;
//    paintPath.addEllipse(paintRect);
//    painter.fillPath(paintPath, QBrush(Qt::cyan));
    painter.drawPixmap(dragArea.x(),dragArea.y(),dragArea.width(),dragArea.height(),
                       QPixmap(":/res/image/ic_fram_head.png"));



//    painter.drawLine(QPoint(WIDTH / 2, LABEL_HEIGHT + 15),
//                     QPoint(WIDTH / 2, height() - LABEL_HEIGHT));
//    paintRect.moveTo(QPoint(0, height() - LABEL_HEIGHT));
//    paintRect.setWidth(WIDTH);
//    double time = frameIndex_ / static_cast<double>(frameRate_);
//    painter.drawText(paintRect, QString::number(time, 'g', 2));
    painter.restore();
}

void FramePointer::mouseDoubleClickEvent(QMouseEvent* e)
{
    if(e->pos().y() >= 93 && e->pos().y() <= 157) {
        emit frameDoubleClicked(frameIndex_);
    }
    QWidget::mouseDoubleClickEvent(e);
}
