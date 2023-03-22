#include "jointselectorwidget.h"
#include "ui_jointselectorwidget.h"
#include<QHBoxLayout>
#include<QLabel>
#include<QPushButton>
#include<QDebug>


JointSelectorWidget::JointSelectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JointSelectorWidget),
    mRobotConnectState(0)
{

    ui->setupUi(this);

    ui->headWidget->setJointId(1);
    ui->leftArmWidget->setJointId(3);
    ui->rightArmWidget->setJointId(2);
    ui->leftLegWidget->setJointId(5);
    ui->rightLegWidget->setJointId(4);
    initConnect();

}




JointSelectorWidget::~JointSelectorWidget()
{
    delete ui;
}

void JointSelectorWidget::setJointName(int joint, QString jointName)
{
    switch(joint){
    case 1:
        ui->headWidget->setJointName(jointName);
        break;
    case 2:
        ui->rightArmWidget->setJointName(jointName);
        break;
    case 3:
        ui->leftArmWidget->setJointName(jointName);
        break;
    case 4:
        ui->rightLegWidget->setJointName(jointName);
        break;
    case 5:
        ui->leftLegWidget->setJointName(jointName);
        break;

    }
}



void JointSelectorWidget::onHeadClicked()
{
    ui->leftArmWidget->setSelected(false);
    ui->rightArmWidget->setSelected(false);
    ui->leftLegWidget->setSelected(false);
    ui->rightLegWidget->setSelected(false);
    ui->headWidget->setSelected(true);
    currSelectedJoint = HEADER;
    emit jointSelected(currSelectedJoint,true);
}

void JointSelectorWidget::onLeftArmClicked()
{
    ui->headWidget->setSelected(false);
    ui->rightArmWidget->setSelected(false);
    ui->leftLegWidget->setSelected(false);
    ui->rightLegWidget->setSelected(false);
    ui->leftArmWidget->setSelected(true);
    currSelectedJoint = LEFTARM;
    emit jointSelected(currSelectedJoint,true);
}

void JointSelectorWidget::onRightArmClicked()
{
    ui->leftArmWidget->setSelected(false);
    ui->headWidget->setSelected(false);
    ui->leftLegWidget->setSelected(false);
    ui->rightLegWidget->setSelected(false);
    ui->rightArmWidget->setSelected(true);
    currSelectedJoint = RIGHTARM;
    emit jointSelected(currSelectedJoint,true);
}

void JointSelectorWidget::onLeftLegClicked()
{
    ui->leftArmWidget->setSelected(false);
    ui->rightArmWidget->setSelected(false);
    ui->headWidget->setSelected(false);
    ui->rightLegWidget->setSelected(false);
    ui->leftLegWidget->setSelected(true);
    currSelectedJoint = LEFTLEG;
    emit jointSelected(currSelectedJoint,true);
}

void JointSelectorWidget::onRightLegClicked()
{
    ui->leftArmWidget->setSelected(false);
    ui->rightArmWidget->setSelected(false);
    ui->leftLegWidget->setSelected(false);
    ui->headWidget->setSelected(false);
    ui->rightLegWidget->setSelected(true);
    currSelectedJoint = RIGHTLEG;
    emit jointSelected(currSelectedJoint,true);
}

void JointSelectorWidget::initConnect()
{
    connect(ui->headWidget,&JointWidget::lockBtnClicked,this,[=](bool isLocked){
        onHeadClicked();
        emit jointLocked(currSelectedJoint,isLocked);
    });

    connect(ui->leftArmWidget,&JointWidget::lockBtnClicked,this,[=](bool isLocked){
        onLeftArmClicked();
        emit jointLocked(currSelectedJoint,isLocked);
    });
    connect(ui->rightArmWidget,&JointWidget::lockBtnClicked,this,[=](bool isLocked){
        onRightArmClicked();
        emit jointLocked(currSelectedJoint,isLocked);
    });
    connect(ui->leftLegWidget,&JointWidget::lockBtnClicked,this,[=](bool isLocked){
        onLeftLegClicked();
        emit jointLocked(currSelectedJoint,isLocked);
    });
    connect(ui->rightLegWidget,&JointWidget::lockBtnClicked,this,[=](bool isLocked){
        onRightLegClicked();
        emit jointLocked(currSelectedJoint,isLocked);
    });

    connect(ui->headWidget,&JointWidget::clicked,this,&JointSelectorWidget::onHeadClicked);
    connect(ui->leftArmWidget,&JointWidget::clicked,this,&JointSelectorWidget::onLeftArmClicked);
    connect(ui->rightArmWidget,&JointWidget::clicked,this,&JointSelectorWidget::onRightArmClicked);
    connect(ui->leftLegWidget,&JointWidget::clicked,this,&JointSelectorWidget::onLeftLegClicked);
    connect(ui->rightLegWidget,&JointWidget::clicked,this,&JointSelectorWidget::onRightLegClicked);
}

void JointSelectorWidget::resetLockState()
{
    ui->headWidget->setLocked(true);
    ui->leftArmWidget->setLocked(true);
    ui->rightArmWidget->setLocked(true);
    ui->leftLegWidget->setLocked(true);
    ui->rightLegWidget->setLocked(true);
}

void JointSelectorWidget::setAllServorsUnlocked(bool isAllUnlocked, int jonitId)
{
    switch(jonitId){
    case 1:
        ui->headWidget->allServorsUnlocked(isAllUnlocked);
        break;
    case 2:
        ui->rightArmWidget->allServorsUnlocked(isAllUnlocked);
        break;
    case 3:
        ui->leftArmWidget->allServorsUnlocked(isAllUnlocked);
        break;
    case 4:
        ui->rightLegWidget->allServorsUnlocked(isAllUnlocked);
        break;
    case 5:
        ui->leftLegWidget->allServorsUnlocked(isAllUnlocked);
        break;

    }
}

bool JointSelectorWidget::getIsLockedForJoint(int jointId)
{
    switch(jointId){
    case 1:
        return ui->headWidget->getLocked();

    case 2:
        return ui->rightArmWidget->getLocked();

    case 3:
        return ui->leftArmWidget->getLocked();

    case 4:
        return ui->rightLegWidget->getLocked();

    case 5:
        return ui->leftLegWidget->getLocked();


    }
}
