#include "robotviewchangewidget.h"
#include "ui_robotviewchangewidget.h"
#include<QRect>
#include<QDebug>
#include<QRect>

RobotViewChangeWidget::RobotViewChangeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotViewChangeWidget)
{
    setAttribute(Qt::WA_StyledBackground);
    ui->setupUi(this);
    connect(ui->viewChangeBtn,&ViewChangeButton::selectDirection,this,&RobotViewChangeWidget::selectDirection);
}

RobotViewChangeWidget::~RobotViewChangeWidget()
{
    delete ui;
}

void RobotViewChangeWidget::setDirection(Direction direction)
{
    ui->viewChangeBtn->setDirection(direction);
}


