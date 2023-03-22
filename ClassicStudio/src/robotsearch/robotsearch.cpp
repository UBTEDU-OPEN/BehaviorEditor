#include "robotsearch.h"
#include<QDebug>
#include<QNetworkAddressEntry>
#include<iostream>
#include<QString>

using namespace std;
#define BROADCAST_PORT_ROBOT  20001
#define BROADCAST_PORT  20002




Robotsearch::Robotsearch(QObject *parent):QObject(parent)
{
    broadcastor_ = new QUdpSocket(this);
    broadcastor1_ = new QUdpSocket(this);

    broadcastor_->bind(BROADCAST_PORT,QUdpSocket::ShareAddress);
    broadcastor1_->bind(BROADCAST_PORT_ROBOT,QUdpSocket::ShareAddress);

    broadcastTimer_ = new QTimer(this);

    mHeartbeatTimer = new QTimer(this);//心跳定时器

    mRetryTimer = new QTimer(this);//指令重试定时器。

    mConnectUdpTimer = new QTimer(this);//udp连接定时器

    mDisconnectUpdTimer = new QTimer(this);//断开udp连接定时器


    robotInfos = new QList<RobotInfo>();


    connect(broadcastTimer_,&QTimer::timeout,this,&Robotsearch::broadcastData);

    connect(mHeartbeatTimer,&QTimer::timeout,this,[=](){
        QString heartBeat = "{\"cmd\":\"heartbeat\"}";
        sendCmdToRobot(heartBeat);

    });

    connect(mRetryTimer,&QTimer::timeout,this,[=](){
        mRetryTimes++;
        udpConnectSuccess();
        if(mRetryTimes > 10){
            stopRetryTimer();
        }
    });
    connect(mConnectUdpTimer,&QTimer::timeout,this,[=](){
       conectUdp();
    });

    connect(mDisconnectUpdTimer,&QTimer::timeout,this,[=](){
       disconnectUdp();
    });

    connect(broadcastor_,&QUdpSocket::readyRead,this,&Robotsearch::processBroadcastResponse);
}

Robotsearch::~Robotsearch()
{
    delete broadcastor_;
    delete broadcastor1_;
    if(broadcastTimer_ != nullptr){
        delete broadcastTimer_;
        broadcastTimer_ = nullptr;
    }
    if(mRetryTimer != nullptr){
        if(mRetryTimer->isActive()){
            stopRetryTimer();
        }
        delete mRetryTimer;
        mRetryTimer = nullptr;
    }
    if(mHeartbeatTimer != nullptr){
        if(mHeartbeatTimer->isActive()){
            stopHeartbeatTimer();
        }
        delete mHeartbeatTimer;
        mHeartbeatTimer = nullptr;
    }
    delete robotInfos;

}



void Robotsearch::startBroadcast(QString sn)
{
    if(!this->broadcastTimer_->isActive()){
        this->mSn = sn;
        robotInfos->clear();
        broadcastNo_ = 0;
        broadcastTimer_->start(1500);
        broadcastData();
    }
}

void Robotsearch::stopBroadcast()
{
    if(this->broadcastTimer_->isActive()){
        broadcastNo_ = 0;
        broadcastTimer_->stop();
    }
}

void Robotsearch::startHeartbeatTimer()
{
    if(!this->mHeartbeatTimer->isActive()){
        mHeartbeatTimer->start(2*1000);
    }
}

void Robotsearch::stopHeartbeatTimer()
{
    if(this->mHeartbeatTimer->isActive()){
        mHeartbeatTimer->stop();
    }
}

void Robotsearch::startRetryTimer()
{
    if(!mRetryTimer->isActive()){
        mRetryTimes = 0;
        mRetryTimer->start(200);
    }

}

void Robotsearch::stopRetryTimer()
{
    if(mRetryTimer->isActive()){
        mRetryTimes = 0;
        mRetryTimer->stop();
    }

}


Robotsearch *Robotsearch::getInstance()
{
    static Robotsearch instance;
    return &instance;
}

void Robotsearch::conectUdp()
{
    QString connectCmd = "{\"cmd\":\"connect\"}";
    sendCmdToRobot(connectCmd);
}

void Robotsearch::setCurrentRobotInfo(RobotInfo robotInfo)
{
    this->mCurrentRobotInfo = robotInfo;
}

void Robotsearch::readAngleToRobot()
{
    QString readAngle = "{\"cmd\":\"servo\",\"type\":\"read_hold\"}";
    sendCmdToRobot(readAngle);
}

void Robotsearch::writeAngleToRobot(QMap<int,double> servos, int runtime)
{
    qDebug() << "Robotsearch::writeAngleToRobot==" << isConnected;
    if(isConnected){
        mModifyServos = servos;
        mRuntime = runtime;
        startRetryTimer();
        udpConnectSuccess();
    }
}

void Robotsearch::readAngleToRobotResult(QString angle)
{
    QStringList strList;
    int pos=0;
    while(pos < angle.length()){
        strList.append(angle.mid(pos,2));
        pos += 2;
    }
    qDebug() << "strList" << strList;
    for(auto it = mModifyServos.cbegin();it != mModifyServos.cend();it++){
        int id = it.key();
        int value = it.value();
        qDebug() << "id===" << id << ";value===" << value;
        QString qnum= QString::number(value,16).right(4);//裁剪字符串前面多余的f
        qDebug() << "qnum====" << qnum;
        strList.replace(id-1,qnum);
    }
    QString modifyAngle = strList.join("");
    qDebug() << "modifyAngle====" << modifyAngle;
    modifyServosToRobot(modifyAngle);
}

void Robotsearch::modifyServosToRobot(QString angle)
{
    QString modifyCmd = QString("{\"cmd\":\"servo\",\"type\":\"write\",\"angle\":\"%1\",\"time\":%2}")
            .arg(angle).arg(mRuntime);
    sendCmdToRobot(modifyCmd);
}

void Robotsearch::sendCmdToRobot(QString cmd)
{

    if(isConnected){
        qDebug() << "Robotsearch::sendCmdToRobot==" << cmd;
        QString ip = mCurrentRobotInfo.ip;
        int port= broadcastor1_->localPort();
        broadcastor_->writeDatagram(cmd.toUtf8(), QHostAddress(ip), port);
    }
}



void Robotsearch::udpConnectSuccess()
{
    QString modifyAngle = "";
    for(auto it = mModifyServos.cbegin();it != mModifyServos.cend();it++){
        int id = it.key();
        int value = it.value();
        if(value == -1){
            modifyAngle+="FF";
        }else {
            QString qnum= QString::number(value,16).right(4).toUpper();//裁剪字符串前面多余的f
            if(qnum.length() == 1){
                qnum = "0" + qnum;
            }
            modifyAngle += qnum;
        }

    }
    modifyServosToRobot(modifyAngle);
}

void Robotsearch::setApolloCommunication(ApolloCommunication *communication)
{
    this->mCommunication = communication;
    connect(mCommunication,&ApolloCommunication::robotConnectionStateChanged,this,&Robotsearch::onRobotConnectionStateChanged);
}

void Robotsearch::startConnectUdpTimer()
{
    if(!mConnectUdpTimer->isActive()){
        mConnectUdpTimer->start(400);
        conectUdp();
    }
}

void Robotsearch::stopConnectUdpTimer()
{
    if(mConnectUdpTimer->isActive()){
        mConnectUdpTimer->stop();
    }
}

void Robotsearch::startDisconnectUdpTimer()
{
    qDebug() << "Robotsearch::startDisconnectUdpTimer==" << mDisconnectUpdTimer->isActive();
    if(!mDisconnectUpdTimer->isActive()){
        mDisconnectUpdTimer->start(100);
        disconnectUdp();
    }
}

void Robotsearch::stopDisconnectUdpTimer()
{
    if(mDisconnectUpdTimer->isActive()){
        mDisconnectUpdTimer->stop();
    }
}

void Robotsearch::disconnectUdp()
{
    QString connectCmd = "{\"cmd\":\"disconnect\"}";
    QString ip = mCurrentRobotInfo.ip;
    int port= broadcastor1_->localPort();
    broadcastor_->writeDatagram(connectCmd.toUtf8(), QHostAddress(ip), port);
}

void Robotsearch::jsonToRobotInfo(const QString &jsonStr)
{
    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(jsonStr.toUtf8(),&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError))
    {
        qDebug() << "Robotsearch::json pares error";
        return;
    }
    QJsonObject jsonObject = document.object();
    qDebug() << "Robotsearch::jsonObject=====" << jsonObject;
    QString cmd = jsonObject["cmd"].toString();
    if(!cmd.isEmpty()){
        if(cmd=="discovery_ack"){
            QString name = jsonObject["name"].toString();
            QString ip = jsonObject["ip"].toString();
            QString status = jsonObject["status"].toString();
            QString sn = "";
            if(!jsonObject["sn"].isNull()){
                sn = jsonObject["sn"].toString();
            }
            RobotInfo info;
            info.cmd = cmd;
            info.name = name;
            info.ip = ip;
            info.status = status;
            info.sn = sn;
            QString ssn = sn.remove(0,sn.size()-4);
            qDebug() << "Robotsearch::discovery broadcastNo_===" << broadcastNo_ << ";size===" << robotInfos->size();
            if(!sn.isEmpty()&& (ssn == mSn)){
                if(!robotInfos->contains(info)){
                    this->robotInfos->push_back(info);
                }
            }
            if(broadcastNo_ == 3 && robotInfos->size() == 1){
                stopBroadcast();
                emit searchRobotResult(robotInfos);
            }else {
                if(broadcastNo_ > 3){
                    if(robotInfos->size() > 0){
                        emit searchRobotResult(robotInfos);
                    }else {
                        if(broadcastNo_ >= 10){
                            stopBroadcast();
                            emit searchTimeOut();
                        }
                    }
                }
            }
        }else if(cmd == "servo_ack"){
            QString type = jsonObject["type"].toString();
            if(type == "read_hold"){
                QString angle = jsonObject["angle"].toString();
                //readAngleToRobotResult(angle);
            }else if(type=="write"){
                stopRetryTimer();
            }
        }else if(cmd == "connect_ack"){
            QString status = jsonObject["status"].toString();
            if(status == "ok"){
                isUdpConnected = true;
                stopConnectUdpTimer();
                startHeartbeatTimer();
            }
        }else if(cmd == "disconnect_ack"){
            if(isConnected){
                startConnectUdpTimer();
            }else {
                stopConnectUdpTimer();
                stopHeartbeatTimer();
                stopDisconnectUdpTimer();
                isUdpConnected = false;
            }
        }
    }

}

void Robotsearch::onRobotConnectionStateChanged(int state)
{
    qDebug() << "Robotsearch::onRobotConnectionStateChanged==" << state;
    if(state == 2){
        isConnected = true;
        startConnectUdpTimer();
        stopDisconnectUdpTimer();
    }else {
        isConnected = false;
        stopHeartbeatTimer();
        startDisconnectUdpTimer();
    }
}



//广播数据
void Robotsearch::broadcastData()
{
    //{"cmd":"discovery", "version":"2.2.0.59", "port":20002}

    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
    for (QNetworkInterface interface : interfaceList) {
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();
        for (QNetworkAddressEntry entry : entryList) {
            QString str = entry.broadcast().toString();
            if (str != "") {
                QString broadcastData_= QString("{\"cmd\":\"discovery\", \"version\":\"%1\", \"port\":%2}")
                        .arg("1.0.0.11").arg(BROADCAST_PORT);
                broadcastor1_->writeDatagram(broadcastData_.toUtf8(), QHostAddress(str), broadcastor1_->localPort());
            }
        }
    }
    broadcastNo_++;
}

//接收机器人广播出来的数据
void Robotsearch::processBroadcastResponse()
{

    QByteArray datagram;
    while (broadcastor_->hasPendingDatagrams()) {
        datagram.resize(broadcastor_->pendingDatagramSize());
        QHostAddress host;
        quint16 port;
        broadcastor_->readDatagram(datagram.data(), datagram.size(), &host, &port);
        QString result =  QString(datagram.data()).toUtf8()
                .replace("\n\t","")
                .replace("\t","")
                .replace("\t","")
                .replace("\n","");
        jsonToRobotInfo(result);
    }
}

