#include "virtualrobotwidget.h"
#include "ui_virtualrobotwidget.h"
#include<QHBoxLayout>
#include<QSpacerItem>
#include"robotviewchangewidget.h"
#include "datacenter.h"
#include<QDebug>
#include"viewchangebutton.h"

VirtualRobotWidget::VirtualRobotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VirtualRobotWidget)
{
    ui->setupUi(this);

    viewChangeWidget = new RobotViewChangeWidget(ui->widget);
    viewChangeWidget->setGeometry(10,10,viewChangeWidget->width(),viewChangeWidget->height());
    connect(viewChangeWidget,&RobotViewChangeWidget::selectDirection,this,&VirtualRobotWidget::onSelectDirection);
    connect(ui->robotWidget,&QRobotView::sigCurrentSelectServoId,this,&VirtualRobotWidget::sigCurrentSelectServoId);
    connect(ui->robotWidget,&QRobotView::sigServoAngleChange,this,&VirtualRobotWidget::sigServoAngleChange);
    connect(ui->bottomWidget,&VirtualRobotBottomWidget::modifyBtnClicked,this,&VirtualRobotWidget::modify);
    connect(ui->bottomWidget,&VirtualRobotBottomWidget::resetBtnClicked,this,[=](){
        ui->robotWidget->switchWatch(QRobotView::CAMERA_BACK);
        ui->robotWidget->stopSetAngle();
        viewChangeWidget->setDirection(Direction::TOP);
        emit resetBtnClicked();
    });
    connect(ui->bottomWidget,&VirtualRobotBottomWidget::addBtnClicked,this,&VirtualRobotWidget::addFrame);
    connect(ui->bottomWidget,&VirtualRobotBottomWidget::frameIntervalChanged,this,&VirtualRobotWidget::frameIntervalChanged);
    //TODO: 需要整理连接
    connect(UBTDataCenter,&DataCenter::keyFrameModified,this,&VirtualRobotWidget::onKeyFrameModified);
}

VirtualRobotWidget::~VirtualRobotWidget()
{
    delete ui;
}

void VirtualRobotWidget::onSelectDirection(int dir)
{
    if(dir == 0){
        ui->robotWidget->switchWatch(QRobotView::CAMERA_BACK);
    }else if(dir == 1){
        ui->robotWidget->switchWatch(QRobotView::CAMERA_FRONT);
    }else if(dir ==2){
        ui->robotWidget->switchWatch(QRobotView::CAMERA_LEFT);
    }else if(dir == 3){
        ui->robotWidget->switchWatch(QRobotView::CAMERA_RIGHT);
    }
}

void VirtualRobotWidget::onJointVauleChange(int nId, double value)
{
    ui->robotWidget->setServoAngle(nId,value);
}




void VirtualRobotWidget::onSetFramAglne(const QMap<int, double> &servos)
{

    auto servoAngles = servos;
    for(auto cit = servoAngles.cbegin(); cit != servoAngles.cend(); ++cit) {
        ui->robotWidget->setServoAngle(cit.key(),cit.value(),false);
    }

}

void VirtualRobotWidget::onSetFramAngleWithRunTime(const QMap<int, double> &servos, int runTime)
{
    auto servoAngles = servos;
    for(auto cit = servoAngles.cbegin(); cit != servoAngles.cend(); ++cit) {
        ui->robotWidget->setServoAngle(cit.key(),cit.value(),runTime,false);
    }
}

void VirtualRobotWidget::onPostureReset(const QMap<int,double>& servos)
{
    Q_UNUSED(servos)
    ui->robotWidget->resetAngle();
}

void VirtualRobotWidget::onKeyFrameModified(bool modified)
{
    ui->bottomWidget->setModifyBtnEnabled(modified);
}

void VirtualRobotWidget::onServoSelected(int servoId)
{
    ui->robotWidget->setSelectedNode(servoId);
}
