#include "controlwidget.h"
#include "ui_controlwidget.h"

#include <QStandardItem>
#include <QModelIndex>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QWheelEvent>
#include <QCursor>
#include <QDebug>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>

#include "servotreeitemdelegate.h"
#include "robotcfg.h"
#include "commondefine.h"
#include "commondialog.h"
#include "datacenter.h"
#include "frameratesettingswidget.h"

ControlWidget::ControlWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ControlWidget)
{
    ui->setupUi(this);
    setObjectName("contrlWidget");
    init();
    QFile styleSheet(":/res/qss/controlWidget.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        setStyleSheet(styleSheet.readAll());
    }
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::init()
{
    ui->scaleSlider->setRange(1, 20);
    ui->scaleSlider->setSingleStep(1);
    ui->scaleSlider->setValue(10);

    addMusic_ = new QPushButton(ui->musicLayerWidget);
    addMusic_->setObjectName("addMusic");
    addMusic_->setIcon(QIcon(":/res/image/ic_time_add.png"));
    addMusic_->setIconSize(QSize(14,14));
    addMusic_->setText(tr("Add"));
    addMusic_->setFixedSize(58,24);

    connect(ui->frameRateSettingBtn, &QPushButton::clicked, this, &ControlWidget::onFrameRateSettingBtnClicked);
    connect(ui->scaleSlider, &QSlider::valueChanged, this, &ControlWidget::onScaleValueChanged);
    connect(ui->scaleDownBtn, &QPushButton::clicked, this, &ControlWidget::scaleDown);
    connect(ui->scaleUpBtn, &QPushButton::clicked, this, &ControlWidget::scaleUp);
    connect(addMusic_,&QPushButton::clicked,this,&ControlWidget::onAddMusicClicked);
    connect(ui->playAll,&QPushButton::clicked,this,&ControlWidget::onPlayClicked);
    connect(ui->stopAll,&QPushButton::clicked,this,&ControlWidget::onStopClicked);
    connect(ui->playAction,&QPushButton::clicked,this,&ControlWidget::onPlayActionClicked);
    connect(ui->stopAction,&QPushButton::clicked,this,&ControlWidget::onStopActionClicked);
    connect(ui->playMusic,&QPushButton::clicked,this,&ControlWidget::onPlayMusicClicked);
    connect(ui->stopMusic,&QPushButton::clicked,this,&ControlWidget::onStopMusicClicked);
    //TODO:
    connect(UBTDataCenter,&DataCenter::playStateChanged,this,&ControlWidget::onPlayerStateChanged);
    connect(UBTDataCenter,&DataCenter::playableStateChanged,this,&ControlWidget::onPlayableStateChanged);
    connect(UBTDataCenter,&DataCenter::playPos,this,&ControlWidget::onPlayPos);
}

void ControlWidget::onPlayPos(int durationMSec)
{
    int durationSec = durationMSec / 1000;
    if(durationMSec%1000 != 0) {
        ++durationSec;
    }
    int durationMin = (durationSec / 60)%60;
    durationSec = durationSec % 60;
    QTime displayTime;
    displayTime.setHMS(0, durationMin, durationSec, 0);
//    ui->timeEdit_2->setDisplayFormat("mm:ss");
    ui->timeEdit_2->setTime(displayTime);
}

void ControlWidget::onDurationChanged(int durationMSec)
{
//    ui->duration->setProperty("value", durationSec);
    int durationSec = durationMSec / 1000;
    if(durationMSec%1000 != 0) {
        ++durationSec;
    }
    int durationMin = (durationSec / 60)%60;
    durationSec = durationSec % 60;
//    ui->duration->setText(QString("%1:%2")
//                          .arg(durationMin, 2, 10, QChar('0'))
//                          .arg(durationSec, 2, 10, QChar('0')));

    QTime displayTime;
    displayTime.setHMS(0, durationMin, durationSec, 0);                                       //初始化数据，时 分 秒 毫秒
    ui->totalTime->setDisplayFormat("mm:ss");
    ui->totalTime->setTime(displayTime);
}

void ControlWidget::onScaleValueChanged(int value)
{
//    qDebug() << "ControlWidget::onScaleValueChanged:" << value;
    emit scaleValueChanged(value);
}

void ControlWidget::onFrameRateSettingBtnClicked()
{
    CommonDialog *dialog = new CommonDialog(tr("Set Frame Rate"),CommonDialog::OkCancelButton,this);
    FrameRateSettingsWidget* widget = new FrameRateSettingsWidget(dialog);
    widget->setDivision(UBTDataCenter->frameDivision());
    dialog->setDisplayWidget(widget);
    connect(dialog,&CommonDialog::sigAccepted,[this,widget](bool accepted){
        if(accepted) {
            int division = widget->division();
            if(division%20 != 0 || 1000%division != 0) {
                CommonDialog* prompt = new CommonDialog(tr("System Prompt"),CommonDialog::OnlyOkButton);
                prompt->setDisplayText(tr("The value needs to be divisible by 20, and 1000 needs to be divisible by it!"));
                prompt->setAbandonBtnText(tr("I Known"));
                prompt->show();
            } else {
                UBTDataCenter->setFrameDivision(division);
            }
        }
    });
    dialog->show();
}

void ControlWidget::onPlayerStateChanged(DataCenter::PlayActionType playType)
{
    switch(playType) {
    case DataCenter::kPlayAll:
        ui->playAll->setProperty("playing",true);
        style()->unpolish(ui->playAll);
        style()->polish(ui->playAll);
        ui->playAction->setEnabled(false);
        ui->stopAction->setEnabled(false);
        ui->playMusic->setEnabled(false);
        ui->stopMusic->setEnabled(false);
        break;
    case DataCenter::kPlayAction:
        ui->playAll->setProperty("playing",true);
        style()->unpolish(ui->playAll);
        style()->polish(ui->playAll);
        ui->playAction->setProperty("playing",true);
        style()->unpolish(ui->playAction);
        style()->polish(ui->playAction);
        ui->playMusic->setEnabled(false);
        ui->stopMusic->setEnabled(false);
        break;
    case DataCenter::kPlayMusic:
        ui->playAll->setProperty("playing",true);
        style()->unpolish(ui->playAll);
        style()->polish(ui->playAll);
        ui->playMusic->setProperty("playing",true);
        style()->unpolish(ui->playMusic);
        style()->polish(ui->playMusic);
        ui->playAction->setEnabled(false);
        ui->stopAction->setEnabled(false);
        break;
    case DataCenter::kPlayNone:
        ui->playAll->setProperty("playing",false);
        style()->unpolish(ui->playAll);
        style()->polish(ui->playAll);
        if(UBTDataCenter->actionPlayable()) {
            ui->playAction->setProperty("playing",false);
            style()->unpolish(ui->playAction);
            style()->polish(ui->playAction);
            ui->playAction->setEnabled(true);
            ui->stopAction->setEnabled(true);
        }
        if(UBTDataCenter->musicPlayable()) {
            ui->playMusic->setProperty("playing",false);
            style()->unpolish(ui->playMusic);
            style()->polish(ui->playMusic);
            ui->playMusic->setEnabled(true);
            ui->stopMusic->setEnabled(true);
        }
        break;
    }
}

void ControlWidget::onPlayClicked()
{
    UBTDataCenter->playOrPause(DataCenter::kPlayAll);
}
void ControlWidget::onStopClicked()
{
    UBTDataCenter->stop(DataCenter::kPlayAll);
}
void ControlWidget::onPlayActionClicked()
{
    UBTDataCenter->playOrPause(DataCenter::kPlayAction);
}
void ControlWidget::onStopActionClicked()
{
    UBTDataCenter->stop(DataCenter::kPlayAction);
}
void ControlWidget::onPlayMusicClicked()
{
    UBTDataCenter->playOrPause(DataCenter::kPlayMusic);
}
void ControlWidget::onStopMusicClicked()
{
    UBTDataCenter->stop(DataCenter::kPlayMusic);
}

void ControlWidget::scaleUp()
{
    auto value = ui->scaleSlider->value() + 1;
    if (value > ui->scaleSlider->maximum()) {
        value = ui->scaleSlider->maximum();
    }
    ui->scaleSlider->setValue(value);
}

void ControlWidget::scaleDown()
{
    auto value = ui->scaleSlider->value() - 1;
    if (value < ui->scaleSlider->minimum()) {
        value = ui->scaleSlider->minimum();
    }
    ui->scaleSlider->setValue(value);
}

void ControlWidget::onAddMusicClicked()
{
    DataCenter::getInstance()->addMusic();
}

void ControlWidget::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QWidget::paintEvent(event);
}

void ControlWidget::onPlayableStateChanged()
{
    bool playable = UBTDataCenter->actionPlayable() || UBTDataCenter->musicPlayable();
    if(playable) {
        ui->playAll->setProperty("playing",false);
        style()->unpolish(ui->playAll);
        style()->polish(ui->playAll);
        ui->playAll->setEnabled(true);
        ui->stopAll->setEnabled(true);
    } else {
        ui->playAll->setEnabled(false);
        ui->stopAll->setEnabled(false);
    }
    if(UBTDataCenter->actionPlayable()) {
        ui->playAction->setProperty("playing",false);
        style()->unpolish(ui->playAction);
        style()->polish(ui->playAction);
        ui->playAction->setEnabled(true);
        ui->stopAction->setEnabled(true);
    } else {
        ui->playAction->setEnabled(false);
        ui->stopAction->setEnabled(false);
    }
    if(UBTDataCenter->musicPlayable()) {
        ui->playMusic->setProperty("playing",false);
        style()->unpolish(ui->playMusic);
        style()->polish(ui->playMusic);
        ui->playMusic->setEnabled(true);
        ui->stopMusic->setEnabled(true);
    } else {
        ui->playMusic->setEnabled(false);
        ui->stopMusic->setEnabled(false);
    }
}

void ControlWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    addMusic_->move(104,68);
}

void ControlWidget::onZoomIn()
{
    scaleUp();
}

void ControlWidget::onZoomOut()
{
    scaleDown();
}
