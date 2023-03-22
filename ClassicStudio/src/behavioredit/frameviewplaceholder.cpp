#include "frameviewplaceholder.h"
#include "ui_frameviewplaceholder.h"

#include <QPaintEvent>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>

FrameViewPlaceHolder::FrameViewPlaceHolder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameViewPlaceHolder)
{
    ui->setupUi(this);
    setFixedHeight(14);
    setStyleSheet("background: #373C49;"); 
}

FrameViewPlaceHolder::~FrameViewPlaceHolder()
{
    delete ui;
}

void FrameViewPlaceHolder::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QWidget::paintEvent(event);
}
