#include "virtualrobotbottomwidget.h"
#include "ui_virtualrobotbottomwidget.h"
#include<QPushButton>
#include<QSize>
#include<QRect>
#include<QDebug>
#include<QPoint>
#include<QMenu>

#include "datacenter.h"
#include "classstudiosettings.h"

VirtualRobotBottomWidget::VirtualRobotBottomWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VirtualRobotBottomWidget),
    frameInterval_(10)
{
    setAttribute(Qt::WA_StyledBackground);
    ui->setupUi(this);
    setTimeBtn = new QPushButton(this);
    setTimeDialog = new SetTimeDialog(this);
    setTimeDialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setTimeDialog->setAttribute(Qt::WA_TranslucentBackground);
    setTimeBtn->setObjectName("setTimeBtn");
    setTimeBtn->setMinimumSize(QSize(8,6));
    setTimeBtn->setMaximumSize(QSize(8,6));


    QString style1 = "QPushButton#setTimeBtn{border-image: url(:/image/res/image/virtualRobotBottom/ic_arrows_down.png);}"
                      "QPushButton#setTimeBtn:pressed{border-image: url(:/image/res/image/virtualRobotBottom/ic_arrows_down_highlight.png);}";
    setTimeBtn->setStyleSheet(style1);

    connect(setTimeBtn,&QPushButton::clicked,this,&VirtualRobotBottomWidget::onSetTimeBtnClick);
    connect(setTimeDialog,&SetTimeDialog::closed,this,&VirtualRobotBottomWidget::onSetTimeDialogClose);
    connect(setTimeDialog,&SetTimeDialog::valueChange,[this](int value){
        frameInterval_ = value;
        UBTDataCenter->setAddFrameInterval(frameInterval_);
//        emit frameIntervalChanged(frameInterval_);
    });
    connect(ui->addBtn,&QToolButton::clicked,this,&VirtualRobotBottomWidget::onAddBtnClicked);
    connect(DataCenter::getInstance(),&DataCenter::playableStateChanged,this,&VirtualRobotBottomWidget::onPlayableStateChanged);
    connect(DataCenter::getInstance(),&DataCenter::playStateChanged,this,&VirtualRobotBottomWidget::onPlayStateChange);
    connect(DataCenter::getInstance(),&DataCenter::robotConnectionStateChanged,[this](int state, QString sn){
        if(state == 2) {
            emit resetBtnClicked();
        }
    });
    ui->stopBtn->setDisabled(true);
    ui->playBtn->setDisabled(true);
    ui->modifyBtn->setDisabled(true);
    ui->stopBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_stop_disable.png"));
    ui->playBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_play_disable.png"));
    ui->modifyBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_change_disable.png"));

}

VirtualRobotBottomWidget::~VirtualRobotBottomWidget()
{
    delete ui;
}

void VirtualRobotBottomWidget::resizeEvent(QResizeEvent *event)
{
    QRect addBtnRect = ui->addBtn->geometry();
    setTimeBtn->setGeometry(addBtnRect.x() + addBtnRect.width()-15,addBtnRect.y() + addBtnRect.height()-15,8,6);
}

void VirtualRobotBottomWidget::setModifyBtnEnabled(bool enabled)
{
    if(enabled){
        ui->modifyBtn->setEnabled(enabled);
        ui->modifyBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_change.png"));
    }else {
        ui->modifyBtn->setEnabled(enabled);
        ui->modifyBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_change_disable.png"));
    }
}

void VirtualRobotBottomWidget::onSetTimeDialogClose()
{
    QString style1 = "QPushButton#setTimeBtn{border-image: url(:/image/res/image/virtualRobotBottom/ic_arrows_down.png);}"
                      "QPushButton#setTimeBtn:pressed{border-image: url(:/image/res/image/virtualRobotBottom/ic_arrows_down_highlight.png);}";
    setTimeBtn->setStyleSheet(style1);
}

void VirtualRobotBottomWidget::onSetTimeBtnClick()
{

    QString style1 = "QPushButton#setTimeBtn{border-image: url(:/image/res/image/virtualRobotBottom/ic_arrows_up.png);}"
                      "QPushButton#setTimeBtn:pressed{border-image: url(:/image/res/image/virtualRobotBottom/ic_arrows_up_highlight.png);}";
    setTimeBtn->setStyleSheet(style1);
    int btnPosX = setTimeBtn->mapToGlobal(QPoint(0, 0)).x();
    int btnPosY = setTimeBtn->mapToGlobal(QPoint(0, 0)).y();
    qDebug() << "btnPosX===" << btnPosX << ";;btnPosY====" << btnPosY;
    QSize size = setTimeDialog->size();
    setTimeDialog->move(btnPosX - (size.width() / 2) + 4 ,btnPosY - setTimeBtn->height() + 20);
    setTimeDialog->show();
    setTimeDialog->raise();
}


void VirtualRobotBottomWidget::onAddBtnClicked()
{
    UBTDataCenter->addFrame();
    ClassStudioSettings settings;
    bool setTimeTips = settings.getSetTimeTips();
    if(!setTimeTips){
        settings.setSetTimeTips(true);
        onSetTimeBtnClick();
    }

}

void VirtualRobotBottomWidget::on_resetBtn_clicked()
{
    emit resetBtnClicked();
}

void VirtualRobotBottomWidget::on_modifyBtn_clicked()
{
    setModifyBtnEnabled(false);
    emit modifyBtnClicked();
}

void VirtualRobotBottomWidget::on_playBtn_clicked()
{
    UBTDataCenter->playOrPause(DataCenter::kPlayAll);
}

void VirtualRobotBottomWidget::on_stopBtn_clicked()
{
    UBTDataCenter->stop(DataCenter::kPlayAll);
}

void VirtualRobotBottomWidget::onPlayableStateChanged()
{
    bool playable = UBTDataCenter->actionPlayable() || UBTDataCenter->musicPlayable();
    if(playable){
        ui->stopBtn->setDisabled(false);
        ui->playBtn->setDisabled(false);
        ui->playBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_play.png"));
        ui->stopBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_stop.png"));
    }else {
        ui->stopBtn->setDisabled(true);
        ui->playBtn->setDisabled(true);
        ui->stopBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_stop_disable.png"));
        ui->playBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_play_disable.png"));
    }
}

void VirtualRobotBottomWidget::onPlayStateChange(DataCenter::PlayActionType playType)
{
    switch (playType) {
    case DataCenter::kPlayAll:
    case DataCenter::kPlayAction:
    case DataCenter::kPlayMusic:
        ui->playBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_suspend.png"));
        ui->addBtn->setEnabled(false);
        ui->resetBtn->setEnabled(false);
        ui->modifyBtn->setEnabled(false);
        ui->addBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_add_disable.png"));
        ui->resetBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_reset_disable.png"));
        ui->modifyBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_change_disable.png"));
        break;
    case DataCenter::kPlayNone:
        ui->playBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_play.png"));
        ui->addBtn->setEnabled(true);
        ui->resetBtn->setEnabled(true);
        ui->addBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_add.png"));
        ui->resetBtn->setIcon(QIcon(":/image/res/image/virtualRobotBottom/ic_reset.png"));
        break;
    default:
        break;
    }
}
