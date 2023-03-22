#include "robotsearchdialog.h"
#include "ui_robotsearchdialog.h"
#include<QDebug>


RobotSearchDialog::RobotSearchDialog(int connectionState,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RobotSearchDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(true);

    QFile styleSheet(":/qss/robotsearch.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        setStyleSheet(styleSheet.readAll());
    }

    if(connectionState == 2){
        ui->stackedWidget->setCurrentIndex(4);
        ui->titleLabel->setText(tr("Robot Connected"));
    }else {
        ui->stackedWidget->setCurrentIndex(0);
        InputSnWidget *inputSnWdg = dynamic_cast<InputSnWidget*>(ui->stackedWidget->currentWidget());
        connect(inputSnWdg,&InputSnWidget::connectBtnClicked,this,&RobotSearchDialog::onConnectBtnClicked);
        connect(Robotsearch::getInstance(),&Robotsearch::searchRobotResult,this,&RobotSearchDialog::onSearchRobotResult);
        connect(Robotsearch::getInstance(),&Robotsearch::searchTimeOut,this,&RobotSearchDialog::onSearchTimeOut);
    }
}

RobotSearchDialog::~RobotSearchDialog()
{
    delete ui;

}

void RobotSearchDialog::closeEvent(QCloseEvent *event)
{
    int index = ui->stackedWidget->currentIndex();
    if(index !=4){
        Robotsearch::getInstance()->stopBroadcast();
    }
    QDialog::closeEvent(event);
}

void RobotSearchDialog::setApolloCommunication(ApolloCommunication* communication)
{
    this->mCommunication = communication;
    Robotsearch::getInstance()->setApolloCommunication(mCommunication);
    connect(mCommunication,&ApolloCommunication::robotConnectionStateChanged,this,&RobotSearchDialog::onRobotConnectionStateChanged);
}

void RobotSearchDialog::setSn(QString sn)
{
    if(!sn.isEmpty()){
        this->mSn = sn;
        int index = ui->stackedWidget->currentIndex();
        if(index == 4){
            ConnectedWidget* connectedWid = dynamic_cast<ConnectedWidget *>(ui->stackedWidget->currentWidget());
            connectedWid->setSn(sn);
            connect(connectedWid,&ConnectedWidget::disconnectBtnClicked,[=](){
                mCommunication->disconnectRobot();
                close();
            });
        }
    }
}

void RobotSearchDialog::onConnectBtnClicked(const QString &sn)
{
    qDebug() << "sn===" << sn;
    Robotsearch::getInstance()->startBroadcast(sn);
    ui->stackedWidget->setCurrentIndex(1);
}

void RobotSearchDialog::onSearchRobotResult(QList<RobotInfo> *result)
{
    int size = result->size();
    qDebug() << "size===" << size;
    if(size == 1){
        RobotInfo robotInfo = result->front();
        this->mSn = robotInfo.sn;
        QString ip = result->front().ip;
        mConnectRobotInfo = robotInfo;
        mCommunication->connectRobot(ip);
    }else {
        int index = ui->stackedWidget->currentIndex();
        if(index != 2){
            ui->stackedWidget->setCurrentIndex(2);
            listWidget = dynamic_cast<RobotListWidget *> (ui->stackedWidget->currentWidget());
            listWidget->setRobotinfoList(result);
            connect(listWidget,&RobotListWidget::listConnectionBtnClick,[=](const RobotInfo info){
                this->mSn = info.sn;
                qDebug() << "ip===" << info.ip;
                Robotsearch::getInstance()->stopBroadcast();
                ui->stackedWidget->setCurrentIndex(1);
                mConnectRobotInfo = info;
                mCommunication->connectRobot(info.ip);
            });
        }else {
            listWidget->setRobotinfoList(result);
        }
    }

}

void RobotSearchDialog::onSearchTimeOut()
{
    qDebug()<< "search time out";
    ui->stackedWidget->setCurrentIndex(0);
    InputSnWidget *inputSnWdg = dynamic_cast<InputSnWidget*>(ui->stackedWidget->currentWidget());
    inputSnWdg->setErroeView(0);
}

void RobotSearchDialog::onRobotConnectionStateChanged(int state)
{
    if(state == 2){
        int index = ui->stackedWidget->currentIndex();
        if(index != 3){
            ui->stackedWidget->setCurrentIndex(3);
            ConnectionSuccessWidget *successWidget = dynamic_cast<ConnectionSuccessWidget*>(ui->stackedWidget->currentWidget());
            connect(successWidget,&ConnectionSuccessWidget::successBtnClicked,[=](){
                ClassStudioSettings settings;
                settings.setConnectedRobotSN(mSn);
                close();
            });
            Robotsearch::getInstance()->setCurrentRobotInfo(mConnectRobotInfo);
        }
    }
    emit connectStateChanged(state,this->mSn);

}



void RobotSearchDialog::on_closeBtn_clicked()
{
    close();
}
