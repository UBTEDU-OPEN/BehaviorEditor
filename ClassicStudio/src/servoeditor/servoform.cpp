#include "servoform.h"
#include "ui_servoform.h"

#include <QDebug>
#include "datacenter.h"
#include<QFile>
#include<QDebug>
#include<QStyle>
#include<QMouseEvent>

const QString kIdStringFormat("ID %1.");

ServoForm::ServoForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServoForm)
    , isProgramSet_(false)
    , servoId_(0)
    , servoAngle_(90)
    , servoAngleBeforePressed_(90)
    , servoLocked_(true)
    , sliderPressed_(false)
{
    ui->setupUi(this);
    ui->lockBtn->setDisabled(true);
    ui->lockBtn->setIcon(QIcon(disenable_url));

    connect(DataCenter::getInstance(),&DataCenter::sigAdjustServo,[this](int selectServoId){
//        if(selectServoId == servoId_){
            if(!servoLocked_){
                servoLocked_ = true;
                setLockState(servoLocked_);
                emit servoLockStateChanged(servoId_,servoLocked_);
            }
//        }
    });

    connect(ui->changeServoSlider,&QSlider::sliderPressed,[this]{
        sliderPressed_ = true;
        servoAngleBeforePressed_ = servoAngle_;
        if(!servoLocked_){
            servoLocked_ = true;
            setLockState(servoLocked_);
            emit servoLockStateChanged(servoId_,servoLocked_);
        }
    });
    connect(ui->changeServoSlider,&QSlider::valueChanged,[this](int value){

        servoAngle_ = value;
        if(sliderPressed_) {
            ui->servoAngle->setValue(value);
            emit servoAngleChanged(servoId_,servoAngle_);
        }
    });
    connect(ui->changeServoSlider,&QSlider::sliderReleased,[this]{

        sliderPressed_ = false;
        if(servoAngleBeforePressed_ != servoAngle_) {
            emit servoAngleChangeFinished(servoId_,servoAngle_);
        }
    });
    ui->widget->installEventFilter(this);

    connect(ui->servoAngle,&SpinBox::editingFinished,[this]{
        if(!servoLocked_){
            servoLocked_ = true;
            setLockState(servoLocked_);
            emit servoLockStateChanged(servoId_,servoLocked_);
        }
        ui->servoAngle->clearFocus();
        servoAngle_ = ui->servoAngle->value();

        emit servoAngleChanged(servoId_,servoAngle_);
        emit servoAngleChangeFinished(servoId_,servoAngle_);

    });


    connect(ui->servoAngle,&SpinBox::stepEventTrigger,[this](int steps){
        qDebug() << "ServoForm SpinBox::stepEventTrigger" << steps;
        if(!servoLocked_){
            servoLocked_ = true;
            setLockState(servoLocked_);
            emit servoLockStateChanged(servoId_,servoLocked_);
        }
        servoAngle_ = ui->servoAngle->value()+steps;
        qDebug() << "ServoForm SpinBox:servoAngle_=11==" << servoAngle_;
        ui->changeServoSlider->setValue(servoAngle_);
        if(editTiemr != nullptr){
            editTiemr->stop();
            editTiemr = nullptr;
        }
        if(editTiemr == nullptr){
            editTiemr = new QTimer(this);
            editTiemr->setSingleShot(true);
            connect(editTiemr,&QTimer::timeout,this,[=](){
                qDebug() << "timeout======";
                ui->servoAngle->clearFocus();
                editTiemr = nullptr;
            });
            editTiemr->start(200);
        }
        emit servoAngleChanged(servoId_,servoAngle_);
    });

    connect(ui->lockBtn,&QPushButton::clicked,[this]{
        ClassStudioSettings settings;
        bool isNoMoreTips = settings.getNoMoreTips();
        if((mBodyId == 4 || mBodyId == 5) && !isNoMoreTips && servoLocked_){
            dDialog = new DangerousDialog(this);
            connect(dDialog,&DangerousDialog::sigSure,this,[=](){
                setLockState(!servoLocked_);
                emit servoLockStateChanged(servoId_,servoLocked_);
            });
            dDialog->exec();
        }else {
            setLockState(!servoLocked_);
            emit servoLockStateChanged(servoId_,servoLocked_);
        }
    });
    connect(DataCenter::getInstance(),&DataCenter::robotConnectionStateChanged,this,&ServoForm::onRobotConnectStateChange);
    connect(DataCenter::getInstance(),&DataCenter::playStateChanged,this,&ServoForm::onPlayStateChange);
}

void ServoForm::setLockState(bool isLock)
{

    servoLocked_ = isLock;
    QString iconUrl;

    if(mConnectState == 2){
        ui->lockBtn->setDisabled(false);
        if(servoLocked_) {
            iconUrl = locked_url;
        } else {
            iconUrl = unlocked_url;
        }
        ui->lockBtn->setIcon(QIcon(iconUrl));
    }else {
        ui->lockBtn->setDisabled(true);
        ui->lockBtn->setIcon(QIcon(disenable_url));
    }

}

int ServoForm::getServoId()
{
    return servoId_;
}

void ServoForm::setSelected(bool isSelected)
{
    if(isSelected){
        ui->widget->setStyleSheet("QWidget#widget{border: 1px solid #00A5FF; border-radius: 0px;}");
    }else {
        ui->widget->setStyleSheet("QWidget#widget{border: node;}");
    }

}

bool ServoForm::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->widget){
        if(event->type()==QEvent::MouseButtonPress){
            QMouseEvent *mouseEvent = (QMouseEvent *)event;
            if(mouseEvent->buttons()&Qt::LeftButton){
                qDebug() << "event==MouseButtonPress====" << event->type();
                emit sigServoSelect(this->servoId_);
            }

        }

    }
    return false;
}

void ServoForm::onRobotConnectStateChange(int state, QString sn)
{
    mConnectState = state;
    if(mConnectState == 2){ // 机器人已连接
        setLockState(servoLocked_);
    }else {
        ui->lockBtn->setDisabled(true);
        ui->lockBtn->setIcon(QIcon(disenable_url));
    }
}

void ServoForm::onPlayStateChange(DataCenter::PlayActionType playType)
{
    switch (playType) {
    case DataCenter::kPlayAll:
    case DataCenter::kPlayAction:
    case DataCenter::kPlayMusic:
        ui->lockBtn->setDisabled(true);
        ui->lockBtn->setIcon(QIcon(disenable_url));
        ui->servoAngle->setDisabled(true);
        ui->changeServoSlider->setDisabled(true);
        break;
    case DataCenter::kPlayNone:
        ui->servoAngle->setDisabled(false);
        ui->changeServoSlider->setDisabled(false);
        if(mConnectState == 2){
            setLockState(true);
        }else {
            ui->lockBtn->setDisabled(true);
            ui->lockBtn->setIcon(QIcon(disenable_url));
        }
        break;
    default:
        break;
    }
}


ServoForm::~ServoForm()
{
    delete ui;
}

void ServoForm::setId(int id)
{
    servoId_ = id;
    QString str = kIdStringFormat.arg(id);
    ui->servoId->setText(str);

    if(id == 1){
        ui->upLabel->setText(tr("Front"));
        ui->downLabel->setText(tr("Back"));
    }else if(id == 4){
        ui->upLabel->setText(tr("Back"));
        ui->downLabel->setText(tr("Front"));
    }else if(id == 5){
        ui->upLabel->setText(tr("Down"));
        ui->downLabel->setText(tr("Up"));
    }else if(id == 10){
        ui->upLabel->setText(tr("Back"));
        ui->downLabel->setText(tr("Front"));
    }else if(id == 11){
        ui->upLabel->setText(tr("Outside"));
        ui->downLabel->setText(tr("Inside"));
    }else if(id == 7){
        ui->upLabel->setText(tr("Outside"));
        ui->downLabel->setText(tr("Inside"));
    }else if(id == 12 || id == 16){
        ui->upLabel->setText(tr("Inside"));
        ui->downLabel->setText(tr("Outside"));
    }else if(id ==2 || id == 3 || id == 6){
        ui->upLabel->setText(tr("Up"));
        ui->downLabel->setText(tr("Down"));
    }else if(id == 17){
        ui->upLabel->setText(tr("Left"));
        ui->downLabel->setText(tr("Right"));
    }else if(id == 11 ){
        ui->upLabel->setText(tr("Right"));
        ui->downLabel->setText(tr("Left"));
    }else if(id ==13 || id ==9 ){
        ui->upLabel->setText(tr("Back"));
        ui->downLabel->setText(tr("Front"));
    }else if(id==14 || id == 15 || id ==8){
        ui->upLabel->setText(tr("Front"));
        ui->downLabel->setText(tr("Back"));
    }else {
        ui->upLabel->setText(tr("Up"));
        ui->downLabel->setText(tr("Down"));
    }

}

void ServoForm::setBodyId(int bodyId)
{
    this->mBodyId = bodyId;
}
void ServoForm::setName(const QString& name)
{
    ui->servoName->setText(name);
}
void ServoForm::setValue(double value)
{
    isProgramSet_ = true;
    ui->servoAngle->setValue(value);
    ui->changeServoSlider->setValue(value);
    isProgramSet_ = false;
}
void ServoForm::setMaxValue(double value)
{
    isProgramSet_ = true;
    ui->servoAngle->setMaximum(value);
    ui->changeServoSlider->setMaximum(static_cast<int>(value));
    isProgramSet_ = false;
}
void ServoForm::setMinValue(double value)
{
    isProgramSet_ = true;
    ui->servoAngle->setMinimum(value);
    ui->changeServoSlider->setMinimum(static_cast<int>(value));
    isProgramSet_ = false;
}

void ServoForm::configStyle()
{

}
