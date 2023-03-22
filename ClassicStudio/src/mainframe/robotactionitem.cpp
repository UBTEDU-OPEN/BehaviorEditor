#include "robotactionitem.h"
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QHBoxLayout>

RobotActionItem::RobotActionItem(QString actionName, QWidget *parent)
    : QWidget(parent)
    , actionName_(actionName)
{
    setLayout(new QHBoxLayout);
    layout()->addWidget(new QLabel(actionName_,this));
}

void RobotActionItem::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "RobotActionItem::mousePressEvent";
    emit itemSelected(actionName_);
}
void RobotActionItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << "RobotActionItem::mouseDoubleClickEvent";
    emit itemTriggered(actionName_);
}

