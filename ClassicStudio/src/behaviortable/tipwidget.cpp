#include "tipwidget.h"
#include <QPainter>
#include <QEvent>
#include <QPaintEvent>

TipWidget::TipWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(16);
    setFixedWidth(16);
}


void TipWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixMap(":/icon/tipicon");
    painter.drawPixmap(rect(),pixMap);
}

void TipWidget::enterEvent(QEvent *)
{
    emit sigEnter();
}

void TipWidget::leaveEvent(QEvent *)
{
    emit sigLeave();
}
