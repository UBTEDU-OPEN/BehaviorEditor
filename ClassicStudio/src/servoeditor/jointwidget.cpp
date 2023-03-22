#include "jointwidget.h"
#include "ui_jointwidget.h"
#include<QPainter>
#include<QDebug>
#include<QStyle>
#include"dangerousdialog.h"
#include"classstudiosettings.h"

JointWidget::JointWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JointWidget),
    isSelected(false),
    isLocked(true),
    mRobotConnectState(false)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    ui->jonitLabel->setStyleSheet("QLabel {background-color: transparent;}");
    ui->centralWidget->setProperty("selected",isSelected);
    ui->lockBtn->setDisabled(true);
    ui->lockBtn->setStyleSheet(this->style_sheet_enabled);



    connect(ui->lockBtn,&QPushButton::clicked,[=](){
       qDebug() << "lockBtn clicked===";
       ClassStudioSettings settings;
       bool isNoMoreTips = settings.getNoMoreTips();
       if((mJointId == 4 || mJointId == 5) && !isNoMoreTips && isLocked){
           dDialog = new DangerousDialog(this);
           connect(dDialog,&DangerousDialog::sigSure,this,&JointWidget::onSigSure);
           dDialog->exec();
       }else {
           onSigSure();
       }

    });

    connect(DataCenter::getInstance(),&DataCenter::sigAdjustServo,[this](int selectServoId){
        allServorsUnlocked(false);
    });
    connect(DataCenter::getInstance(),&DataCenter::robotConnectionStateChanged,this,&JointWidget::onRobotConnectStateChange);
//    connect(DataCenter::getInstance(),&DataCenter::playStateChanged,this,&JointWidget::onChangePlayState);

    connect(DataCenter::getInstance(),&DataCenter::playStateChanged,this,&JointWidget::onPlayStateChange);

}

void JointWidget::mousePressEvent(QMouseEvent *ev)
{
    if(!isSelected){
        isSelected = !isSelected;
        ui->centralWidget->style()->unpolish(ui->centralWidget);
        ui->centralWidget->setProperty("selected",isSelected);
        ui->centralWidget->style()->polish(ui->centralWidget);
        emit clicked(this->isSelected);
    }
    QWidget::mousePressEvent(ev);
}

void JointWidget::mouseReleaseEvent(QMouseEvent *ev)
{

    QWidget::mouseReleaseEvent(ev);
}


JointWidget::~JointWidget()
{
    delete ui;
}

void JointWidget::setJointName(QString jointName)
{
    ui->jonitLabel->setText(jointName);
}

void JointWidget::setJointId(int jointId)
{
    this->mJointId = jointId;
}

void JointWidget::setSelected(bool isSelected)
{
    this->isSelected = isSelected;
    ui->centralWidget->style()->unpolish(ui->centralWidget);
    ui->centralWidget->setProperty("selected",isSelected);
    ui->centralWidget->style()->polish(ui->centralWidget);
}

void JointWidget::setLocked(bool locked)
{
    isLocked = locked;
    if(isLocked){
        ui->lockBtn->setStyleSheet(this->style_sheet_locked);
    }else {
        ui->lockBtn->setStyleSheet(this->style_sheet_unlocked);
    }
    emit lockBtnClicked(isLocked);
}

bool JointWidget::getLocked()
{
    return isLocked;
}

void JointWidget::allServorsUnlocked(bool allUnlocked)
{
    isLocked = !allUnlocked;
    if(isLocked){
        ui->lockBtn->setStyleSheet(this->style_sheet_locked);
    }else {
        ui->lockBtn->setStyleSheet(this->style_sheet_unlocked);
    }
}

void JointWidget::onSigSure()
{
    isLocked = !isLocked;
    if(isLocked){
        ui->lockBtn->setStyleSheet(this->style_sheet_locked);
    }else {
        ui->lockBtn->setStyleSheet(this->style_sheet_unlocked);
    }
    emit lockBtnClicked(isLocked);
}

void JointWidget::onRobotConnectStateChange(int state, QString sn)
{
    if(state == 2){ // 机器人已连接

        ui->lockBtn->setDisabled(false);

        if(isLocked){
            ui->lockBtn->setStyleSheet(this->style_sheet_locked);
        }else {
            ui->lockBtn->setStyleSheet(this->style_sheet_unlocked);
        }
    }else { //机器人没连接

        ui->lockBtn->setDisabled(true);
        ui->lockBtn->setStyleSheet(this->style_sheet_enabled);

    }
}

void JointWidget::onPlayStateChange(DataCenter::PlayActionType playType)
{
    int connectState = DataCenter::getInstance()->getCurrentConnectState();
    if(connectState == 2){
        switch (playType) {
        case DataCenter::kPlayAll:
        case DataCenter::kPlayAction:
        case DataCenter::kPlayMusic:
            ui->lockBtn->setDisabled(true);
            ui->lockBtn->setStyleSheet(this->style_sheet_enabled);
            break;
        case DataCenter::kPlayNone:
            ui->lockBtn->setDisabled(false);
            isLocked = true;
            ui->lockBtn->setStyleSheet(this->style_sheet_locked);
            break;
        default:
            break;
        }
    }else {
        ui->lockBtn->setDisabled(true);
        ui->lockBtn->setStyleSheet(this->style_sheet_enabled);
    }
}



