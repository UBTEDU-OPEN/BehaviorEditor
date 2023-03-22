#include "layerbgwidget.h"

#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QPen>
#include <QStyleOption>
#include <QStyle>


static const QMap<int, QString> layer2name {
    {0,"a"},
    {1,"h"},{2,"s1"},{3,"s2"},
    {4,"ra"},{5,"s3"},{6,"s4"},{7,"s5"},{8,"s6"},{9,"s7"},
    {10,"la"},{11,"s8"},{12,"s9"},{13,"s10"},{14,"s11"},{15,"s12"},
    {16,"rl"},{17,"s13"},{18,"s14"},{19,"15"},{20,"s16"},{21,"s17"},{22,"s18"},
    {23,"ll"},{24,"s19"},{25,"s20"},{26,"s21"},{27,"s22"},{28,"s23"},{29,"s24"},
    {30,"m"},{31,"exp"},{32,"led"}
};


LayerBgWidget::LayerBgWidget(int layerId, QString layerName, QWidget* parent)
    : QWidget(parent)
    , layerId_(layerId)
    , layerName_(layerName)
{
//    auto lb = new QLabel(layerName, this);
//    lb->move(0, 10);
    setObjectName("LayerBgWidget");
    setStyleSheet("QWidget#LayerBgWidget{background: #2C313D;}");
}

void LayerBgWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        emit sigRightButtonPressed(layerId_);
    }
    QWidget::mousePressEvent(event);

//    if(event->button() == Qt::LeftButton) {
//        drawSelectionRect_ = true;
//        pos1 = pos2 = event->pos();
//    }
}

void LayerBgWidget::mouseMoveEvent(QMouseEvent* e)
{
    if(drawSelectionRect_) {
        pos2 = e->pos();
        update();
    }
    QWidget::mouseMoveEvent(e);
}

void LayerBgWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit sigLeftButtonReleased();
    }
    QWidget::mouseReleaseEvent(event);

    if(drawSelectionRect_) {
        drawSelectionRect_ = false;
        update();
    }
}

void LayerBgWidget::paintEvent(QPaintEvent * event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QWidget::paintEvent(event);

    if(drawSelectionRect_) {
        p.save();
        p.setPen(QColor(0x30,0xE0,0xFE));
        QRect rect(pos1,pos2);
        p.drawRect(rect);
        p.restore();
    }
}

void LayerBgWidget::setDrawRect(bool draw,QPoint pos)
{
    drawSelectionRect_ = draw;
    if(draw) {
        pos1 = pos2 = pos;
    }
}
