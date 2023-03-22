#include "toolbar.h"
#include "ui_toolbar.h"

#include <QDebug>
#include <QFileDialog>
#include <QDialog>
#include <QLineEdit>
#include "apollocommunication.h"
#include "datacenter.h"
#include "volumeslider.h"

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToolBar)
{
    ui->setupUi(this);
    configStyle();
    initConnections();
}

ToolBar::~ToolBar()
{
    delete ui;
}

void ToolBar::configStyle()
{
    QFile styleSheet(":/qss/res/qss/toolbar.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        setStyleSheet(styleSheet.readAll());
    }
    ui->horizontalLayout->setSpacing(0);
    ui->horizontalLayout->setContentsMargins(22,0,20,0);
    ui->separator_1->setFixedSize(1,16);
    ui->separator_2->setFixedSize(1,16);
    ui->newBtn->setFixedSize(64,34);
    ui->openBtn->setFixedSize(64,34);
    ui->saveBtn->setFixedSize(64,34);
    ui->saveAsBtn->setFixedSize(80,34);
    ui->helpBtn->setFixedSize(64,34);

    //	m_mainMenu = new QMenu(this);
    //	m_mainMenu->addAction(QIcon(""), tr("New"), this, &TitleBar::sigNewFile);
    //	m_mainMenu->addAction(QIcon(""), tr("Open"), this, &TitleBar::sigOpenFile);
    //	m_mainMenu->addAction(QIcon(""), tr("Save"), this, &TitleBar::sigSaveFile);
    //	m_mainMenu->addAction(QIcon(""), tr("SaveAs"), this, &TitleBar::sigSaveAs);
    ui->newBtn->setIconSize(QSize(24,24));
    ui->openBtn->setIconSize(QSize(24,24));
    ui->saveBtn->setIconSize(QSize(24,24));
    ui->saveAsBtn->setIconSize(QSize(24,24));
    ui->helpBtn->setIconSize(QSize(24,24));

    ui->newBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_new.png"));
    ui->openBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_open.png"));
    ui->saveBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_save.png"));
    ui->saveAsBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_save_as.png"));
    ui->helpBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_help.png"));
    ui->powerBtn->setFixedSize(65,34);
    ui->powerBtn->setIconSize(QSize(24,24));
    ui->powerBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_power_20.png"));
    ui->powerBtn->setText("0%");
    ui->volumeBtn->setFixedSize(75,34);
    ui->volumeBtn->setIconSize(QSize(24,24));
    ui->volumeBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_volume.png"));
    ui->connectedBtn->setFixedSize(150,34);
    ui->connectedBtn->setIconSize(QSize(25,25));
    ui->connectedBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_robot.png"));
    ui->connectedBtn->setText(tr("Unconnected"));

    ui->powerBtn->setVisible(false);
    ui->volumeBtn->setVisible(false);
    ui->separator_2->setVisible(false);

}

void ToolBar::initConnections()
{
    connect(ui->connectedBtn,&QPushButton::clicked,[this]{
        emit robotBtnClicked();
    });
    connect(ui->newBtn,&QPushButton::clicked,this,&ToolBar::sigNewFile);
    connect(ui->openBtn,&QPushButton::clicked,this,&ToolBar::sigOpenFile);
    connect(ui->saveBtn,&QPushButton::clicked,this,&ToolBar::sigSaveFile);
    connect(ui->saveAsBtn,&QPushButton::clicked,this,&ToolBar::sigSaveAs);
    connect(ui->helpBtn,&QPushButton::clicked,this,&ToolBar::sigHelp);

//    connect(ui->mode,&QPushButton::clicked,[this]{
//        emit modeBtnClicked();
//    });
    connect(ui->volumeBtn,&QPushButton::clicked,[this]{
        auto slider = new VolumeSlider(volume_,this);
        connect(slider,&VolumeSlider::volumeChanged,[this](int volume){
            volume_ = volume;
            DataCenter::getInstance()->setDeviceVolume(volume);
        });
        auto pos = mapToGlobal(ui->volumeBtn->geometry().bottomLeft());
        slider->move(pos);
        slider->show();
    });

}

void ToolBar::connectionStateChanged(int state,QString sn)
{
    switch (state) {
    case 0:
        ui->connectedBtn->setText(tr("Unconnected"));
        ui->connectedBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_robot.png"));
        ui->powerBtn->setVisible(false);
        ui->volumeBtn->setVisible(false);
        ui->separator_2->setVisible(false);
        break;
    case 1:
        ui->connectedBtn->setText(tr("Connecting"));
        ui->powerBtn->setVisible(false);
        ui->volumeBtn->setVisible(false);
        ui->separator_2->setVisible(false);
        break;
    case 2:
    {
        QString s = sn.remove(0,sn.size() -4);

        QString ssn = QString(tr("Connected(%1)")).
                    arg(s);
        ui->connectedBtn->setText(ssn);
        ui->connectedBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_robot_highlight.png"));
        ui->powerBtn->setVisible(true);
        ui->volumeBtn->setVisible(true);
        ui->separator_2->setVisible(true);
    }
        break;
    default:
        break;
    }
}

void ToolBar::chargingStateChanged(int inCharging)
{
    charging_ = inCharging;
}

void ToolBar::batteryPercentChanged(int percent)
{
    batteryPercent_ = percent;
    if(percent < 20) {
        if(charging_) {
            ui->powerBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_power_20_charging.png"));
        } else {
            ui->powerBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_power_20.png"));
        }
    } else if(percent >= 20 && percent < 40) {
        if(charging_) {
            ui->powerBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_power_40_charging.png"));
        } else {
            ui->powerBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_power_40.png"));
        }
    } else if(percent >= 40 && percent < 60) {
        if(charging_) {
            ui->powerBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_power_60_charging.png"));
        } else {
            ui->powerBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_power_60.png"));
        }
    } else if(percent >= 60 && percent < 80) {
        if(charging_) {
            ui->powerBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_power_80_charging.png"));
        } else {
            ui->powerBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_power_80.png"));
        }
    } else if(percent >= 80 && percent <= 100) {
        if(charging_) {
            ui->powerBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_power_100_charging.png"));
        } else {
            ui->powerBtn->setIcon(QIcon(":/image/res/image/toolBar/ic_power_100.png"));
        }
    }
    QString text = QString("%1%").arg(percent);
    ui->powerBtn->setText(text);
}

void ToolBar::volumeChanged(int volume)
{
    volume_ = volume;
}
