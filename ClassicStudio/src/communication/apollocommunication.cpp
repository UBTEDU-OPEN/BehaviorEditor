#include "apollocommunication.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>

const QString apolloHostFormat = "http://%1:9090/";
const QString apolloBasePath = "v1";

const int kConnectionCheckingInterval = 30; //second
const int kGetRobotServosInterval = 1000; //milli-second
const int kConnectionCheckingRetryTimes = 2; //重试2次，共试3次
const int kGetDevicesInfoIterval = 60*1000; //1分钟

ApolloCommunication::ApolloCommunication(QObject* parent)
    : QObject(parent)
    , robotConnectionState_(RobotUnconnected)
    , robotDisconnecting_(false)
    , devicesApi_(nullptr)
    , motionsApi_(nullptr)
    , servosApi_(nullptr)
    , mediaApi_(nullptr)
    , getRobotServosTimer_(nullptr)
{
    connectionCheckingTimer_ = new QTimer(this);
    connect(connectionCheckingTimer_,&QTimer::timeout,this,&ApolloCommunication::onConnectionCheckingTimeout);
    getRobotServosTimer_ = new QTimer(this);
    connect(getRobotServosTimer_,&QTimer::timeout,this,&ApolloCommunication::onGetRobotServosTimeout);
    getDevicesInfoTimer_ = new QTimer(this);
    connect(getDevicesInfoTimer_,&QTimer::timeout,this,&ApolloCommunication::onGetDevicesInfoTimeout);
}

//仅支持hts,mp3,zip[,wav]
void ApolloCommunication::postFileToRobot(const QString &filePath)
{
    QFileInfo info(filePath);
    if(info.suffix().compare("hts",Qt::CaseInsensitive) == 0 || info.suffix().compare("zip",Qt::CaseInsensitive) == 0) {
        if(motionsApi_ != nullptr) {
            Swagger::SWGHttpRequestInputFileElement postFile;
            postFile.local_filename = filePath;
            motionsApi_->postMotions(&postFile);
        }
    } else if(info.suffix().compare("mp3",Qt::CaseInsensitive) == 0 || info.suffix().compare("wav",Qt::CaseInsensitive) == 0) {
        if(mediaApi_ != nullptr) {
            Swagger::SWGHttpRequestInputFileElement postFile;
            postFile.local_filename = filePath;
            mediaApi_->postMediaMusic(&postFile);
        }
    }
}

void ApolloCommunication::putCommand(const QString& name
                                     , const QString& direction
                                     , int repeatCount
                                     , const QString& speed
                                     , const QString& operation)
{
    if(motionsApi_ != nullptr && !name.isEmpty()) {
        Swagger::SWGMotionsOperation body;
        auto motionPtr = body.getMotion();
        auto namePtr = motionPtr->getName();
        *namePtr = name;
        auto directionPtr = motionPtr->getDirection();
        *directionPtr = direction;
        auto speedPtr = motionPtr->getSpeed();
        *speedPtr = speed;
        motionPtr->setRepeat(repeatCount);
        auto operationPtr = body.getOperation();
        *operationPtr = operation;
        body.setOperation(operationPtr);
        qDebug() << body.asJson();
        motionsApi_->putMotions(body);
    }
}

void ApolloCommunication::connectRobot(const QString& addr)
{
    //初始化
    currentRobotAddr_ = addr;
    connectionCheckingRetryTimes_ = kConnectionCheckingRetryTimes;
    robotDisconnecting_ = false;
    syncServoNames_.clear();
    if(getRobotServosTimer_->isActive()) { getRobotServosTimer_->stop(); }
    if(connectionCheckingTimer_->isActive()) { connectionCheckingTimer_->stop(); }
    if(!getDevicesInfoTimer_->isActive()) {
        getDevicesInfoTimer_->start(kGetDevicesInfoIterval);
    }
    QString apolloHost = apolloHostFormat.arg(currentRobotAddr_);
    qDebug() << apolloHost;
    if(devicesApi_) { delete devicesApi_; devicesApi_ = nullptr; }
    if(motionsApi_) { delete motionsApi_; motionsApi_ = nullptr; }
    if(servosApi_) { delete servosApi_; servosApi_ = nullptr; }
    if(mediaApi_) { delete mediaApi_; mediaApi_ = nullptr; }
    devicesApi_ = new Swagger::SWGDevicesApi(apolloHost,apolloBasePath);
    motionsApi_ = new Swagger::SWGMotionsApi(apolloHost,apolloBasePath);
    servosApi_ = new Swagger::SWGServosApi(apolloHost,apolloBasePath);
    mediaApi_ = new Swagger::SWGMediaApi(apolloHost,apolloBasePath);
    connect(devicesApi_,&Swagger::SWGDevicesApi::getDevicesBatterySignal,this,&ApolloCommunication::onDevicesBatterySignal);
    connect(devicesApi_,&Swagger::SWGDevicesApi::getDevicesBatterySignalEFull,this,&ApolloCommunication::onDevicesBatterySignalEFull);
    connect(devicesApi_,&Swagger::SWGDevicesApi::getDevicesVolumeSignal,this,&ApolloCommunication::onDevicesVolumeSignal);
    connect(devicesApi_,&Swagger::SWGDevicesApi::getDevicesVolumeSignalEFull,this,&ApolloCommunication::onDevicesVolumeSignalEFull);
    connect(devicesApi_,&Swagger::SWGDevicesApi::putDevicesVolumeSignal,this,&ApolloCommunication::onPutDevicesVolumeSignal);
    connect(devicesApi_,&Swagger::SWGDevicesApi::putDevicesVolumeSignalEFull,this,&ApolloCommunication::onPutDevicesVolumeSignalEFull);
    connect(motionsApi_,&Swagger::SWGMotionsApi::postMotionsSignal,this,&ApolloCommunication::onPostMotionsSignal);
    connect(motionsApi_,&Swagger::SWGMotionsApi::postMotionsSignalEFull,this,&ApolloCommunication::onPostMotionsSignalEFull);
    connect(motionsApi_,&Swagger::SWGMotionsApi::putMotionsSignal,this,&ApolloCommunication::onPutMotionsSignal);
    connect(motionsApi_,&Swagger::SWGMotionsApi::putMotionsSignalEFull,this,&ApolloCommunication::onPutMotionsSignalEFull);
    connect(motionsApi_,&Swagger::SWGMotionsApi::getMotionsListSignal,this,&ApolloCommunication::onGetMotionsListSignal);
    connect(motionsApi_,&Swagger::SWGMotionsApi::getMotionsListSignalEFull,this,&ApolloCommunication::onGetMotionsListSignalEFull);
    connect(motionsApi_,&Swagger::SWGMotionsApi::deleteMotionsMusicSignal,this,&ApolloCommunication::onDeleteMotionsMusicSignal);
    connect(motionsApi_,&Swagger::SWGMotionsApi::deleteMotionsMusicSignalEFull,this,&ApolloCommunication::onDeleteMotionsMusicSignalEFull);
    connect(servosApi_,&Swagger::SWGServosApi::putServosAnglesSignal,this,&ApolloCommunication::onPutServosAnglesSignal);
    connect(servosApi_,&Swagger::SWGServosApi::putServosAnglesSignalEFull,this,&ApolloCommunication::onPutServosAnglesSignalEFull);
    connect(servosApi_,&Swagger::SWGServosApi::getServosAnglesSignal,this,&ApolloCommunication::onGetServosAnglesSignal);
    connect(servosApi_,&Swagger::SWGServosApi::getServosAnglesSignalEFull,this,&ApolloCommunication::onGetServosAnglesSignalEFull);
    connect(servosApi_,&Swagger::SWGServosApi::putServosModeSignal,this,&ApolloCommunication::onPutServosModeSignal);
    connect(servosApi_,&Swagger::SWGServosApi::putServosModeSignalEFull,this,&ApolloCommunication::onPutServosModeSignalEFull);
    connect(servosApi_,&Swagger::SWGServosApi::getServosModeSignal,this,&ApolloCommunication::onGetServosModeSignal);
    connect(servosApi_,&Swagger::SWGServosApi::getServosModeSignalEFull,this,&ApolloCommunication::onGetServosModeSignalEFull);
    connect(mediaApi_,&Swagger::SWGMediaApi::getMediaMusicListSignal,this,&ApolloCommunication::onGetMediaMusicListSignal);
    connect(mediaApi_,&Swagger::SWGMediaApi::getMediaMusicListSignalEFull,this,&ApolloCommunication::onGetMediaMusicListSignalEFull);
    connect(mediaApi_,&Swagger::SWGMediaApi::postMediaMusicSignal,this,&ApolloCommunication::onPostMusicSignal);
    connect(mediaApi_,&Swagger::SWGMediaApi::postMediaMusicSignalEFull,this,&ApolloCommunication::onPostMusicSignalEFull);

    //判断连接
    changeRobotConnectionState(RobotConnecting);
    checkConnection(); //先判断一次
    getDevicesBattery();
    getDevicesVolume();
}

void ApolloCommunication::disconnectRobot()
{
    changeRobotConnectionState(RobotUnconnected);
    if(connectionCheckingTimer_->isActive()) {
        connectionCheckingTimer_->stop();
    }
    if(getRobotServosTimer_->isActive()) {
        getRobotServosTimer_->stop();
    }
    if(getDevicesInfoTimer_->isActive()) {
        getDevicesInfoTimer_->stop();
    }
    if(robotConnectionState_ == RobotConnecting) {
        robotDisconnecting_ = true;
    }
    currentRobotAddr_.clear();
    syncServoNames_.clear();
    delete devicesApi_; devicesApi_ = nullptr;
    delete motionsApi_; motionsApi_ = nullptr;
    delete servosApi_; servosApi_ = nullptr;
    delete mediaApi_; mediaApi_ = nullptr;
}

void ApolloCommunication::putServosAngles(const QMap<QString,double>& servos,int runtime)
{
    if(servosApi_) {
        Swagger::SWGServosAnglesRequest body;
        QJsonObject rootObj;
        QJsonObject anglesObj;
        for(auto cit = servos.cbegin(); cit != servos.cend(); cit++) {
            anglesObj[cit.key()] = static_cast<int>(cit.value());
        }
        rootObj["angles"] = anglesObj;
        rootObj["runtime"] = runtime;
        body.fromJsonObject(rootObj);
        servosApi_->putServosAngles(body);
        removeSyncServoNames(servos.keys().toSet());
    }
}

void ApolloCommunication::getServosAngles(const QList<QString>& servoIdNames)
{
    if(!servoIdNames.empty() && servosApi_) {
        QList<QString> servosNames = servoIdNames;
        QList<QString*> servosNamePtrs;
        for(int i = 0; i < servosNames.size(); i++) {
            servosNamePtrs.push_back(&servosNames[i]);
        }
        servosApi_->getServosAngles(&servosNamePtrs);
    }
}

void ApolloCommunication::putServosMode(const QList<QString>& servoNames, bool isLock)
{
    if(servosApi_) {
        Swagger::SWGServosModeRequest body;
        auto servosList = body.getServos();
        for(auto servoName : servoNames) {
            Swagger::SWGServosList* servo = new Swagger::SWGServosList;
            auto namePtr = servo->getName();
            *namePtr = servoName;
            servosList->push_back(servo);
        }
        body.setServos(servosList);
        auto mode = body.getMode();
        if(isLock) {    
            *mode = "work";
            removeSyncServoNames(servoNames.toSet());
        } else {
            *mode = "program";
            syncServoNames_ += servoNames.toSet();
            if(!getRobotServosTimer_->isActive()) {
                getRobotServosTimer_->start(kGetRobotServosInterval);
            }
        }
        body.setMode(mode);
        servosApi_->putServosMode(body);
    }
}

void ApolloCommunication::getServosMode(const QList<QString>& servoNames)
{
    if(servosApi_) {
        QList<QString> servosNames = servoNames;
        QList<QString*> servosNamePtrs;
        for(int i = 0; i < servosNames.size(); i++) {
            servosNamePtrs.push_back(&servosNames[i]);
        }
        servosApi_->getServosMode(&servosNamePtrs);
    }
}

void ApolloCommunication::getMotionsList()
{
    if(motionsApi_) {
        motionsApi_->getMotionsList();
    }
}

void ApolloCommunication::deleteMotions(const QString& actionName)
{
    if(motionsApi_) {
        Swagger::SWGName name;
        QString* namePtr = name.getName();
        *namePtr = actionName;
        name.setName(namePtr);
        motionsApi_->deleteMotionsMusic(name);
    }
}

void ApolloCommunication::getMediaMusicList()
{
    if(mediaApi_) {
        mediaApi_->getMediaMusicList();
    }
}

void ApolloCommunication::changeRobotConnectionState(RobotConnectionState state)
{
    robotConnectionState_ = state;
    emit robotConnectionStateChanged(state);
}

void ApolloCommunication::onDevicesBatterySignal(Swagger::SWGDevicesBatteryResponse* summary)
{
    qDebug() << "ApolloCommunication::onDevicesBatterySignal" << summary->getCode() << summary->getData()->getCharging() << summary->getData()->getPercent();

    if(summary->getCode() == 0) {
        emit batteryState(summary->getData()->getCharging(),summary->getData()->getPercent());
    }
}

void ApolloCommunication::onDevicesBatterySignalEFull(Swagger::SWGHttpRequestWorker* worker
                                                      , QNetworkReply::NetworkError error_type
                                                      , QString& error_str)
{
    qDebug() << "ApolloCommunication::onDevicesBatterySignalEFull" << error_str;
}

void ApolloCommunication::onPostMotionsSignal(Swagger::SWGCommonResponse* summary)
{
    qDebug() << "ApolloCommunication::postFile" << summary->getCode() << *summary->getMsg();
    if(summary->getCode() == 0) {
        emit sendFileResult(true);
        getMotionsList();
    } else {
        emit sendFileResult(false);
    }
}

void ApolloCommunication::onPostMotionsSignalEFull(Swagger::SWGHttpRequestWorker* worker
                                                   , QNetworkReply::NetworkError error_type
                                                   , QString& error_str)
{
    qDebug() << "ApolloCommunication::onPostMotionsSignalEFull" << error_type << error_str;
    emit sendFileResult(false);
}

void ApolloCommunication::onPostMusicSignal(Swagger::SWGCommonResponse* summary)
{
    qDebug() << "ApolloCommunication::onPostMusicSignal" << summary->getCode() << *summary->getMsg();
    if(summary->getCode() == 0) {
        emit sendFileResult(true);
//        getMotionsList();
    } else {
        emit sendFileResult(false);
    }
}

void ApolloCommunication::onPostMusicSignalEFull(Swagger::SWGHttpRequestWorker* worker
                                                   , QNetworkReply::NetworkError error_type
                                                   , QString& error_str)
{
    qDebug() << "ApolloCommunication::onPostMusicSignalEFull" << error_type << error_str;
//    emit sendFileResult(false);
}

void ApolloCommunication::onPutMotionsSignal(Swagger::SWGRuntimeResponse* summary)
{

}

void ApolloCommunication::onPutMotionsSignalEFull(Swagger::SWGHttpRequestWorker* worker
                                                  , QNetworkReply::NetworkError error_type
                                                  , QString& error_str)
{
    qDebug() << "ApolloCommunication::onPostMotionsSignalEFull" << error_type << error_str;
}

void ApolloCommunication::onPutServosAnglesSignal(Swagger::SWGServosResultResponse* summary)
{
    qDebug() << "ApolloCommunication::onPutServosAnglesSignal" << summary->getCode() << summary->getMsg();
}

void ApolloCommunication::onPutServosAnglesSignalEFull(Swagger::SWGHttpRequestWorker* worker
                                                     , QNetworkReply::NetworkError error_type
                                                     , QString& error_str)
{
    qDebug() << "ApolloCommunication::putServosAnglesSignalEFull" << error_type << error_str;
}

void ApolloCommunication::onGetServosAnglesSignal(Swagger::SWGServosAnglesResponse* summary)
{
    if(summary->getCode() == 0) {
        auto servosAnglesJsonObject = summary->getData()->asJsonObject();
        QMap<QString,int> servosAngles;
        for(auto cit = servosAnglesJsonObject.constBegin(); cit != servosAnglesJsonObject.constEnd(); cit++) {
            if(syncServoNames_.contains(cit.key())){
                servosAngles.insert(cit.key(),cit.value().toInt());
            }
        }
        if(!syncServoNames_.empty()){
            emit robotServosAngles(servosAngles);
        }
    }
}

void ApolloCommunication::onGetServosAnglesSignalEFull(Swagger::SWGHttpRequestWorker* worker
                                                     , QNetworkReply::NetworkError error_type
                                                     , QString& error_str)
{
    qDebug() << "ApolloCommunication::getServosAnglesSignalEFull" << error_type << error_str;
}

void ApolloCommunication::onPutServosModeSignal(Swagger::SWGServosResultResponse* summary)
{

}

void ApolloCommunication::onPutServosModeSignalEFull(Swagger::SWGHttpRequestWorker* worker
                                                     , QNetworkReply::NetworkError error_type
                                                     , QString& error_str)
{
    qDebug() << "ApolloCommunication::onPutServosModeSignalEFull" << error_type << error_str;
}

void ApolloCommunication::onGetServosModeSignal(Swagger::SWGServosModeResponse* summary)
{
    if(summary->getCode() == 0) {
        auto servosAnglesJsonObject = summary->getData()->asJsonObject();
        QMap<QString,int> servosAngles;
        for(auto cit = servosAnglesJsonObject.constBegin(); cit != servosAnglesJsonObject.constEnd(); cit++) {
            servosAngles.insert(cit.key(),cit.value().toInt());
        }
        emit robotServosAngles(servosAngles);
    }
}

void ApolloCommunication::onGetServosModeSignalEFull(Swagger::SWGHttpRequestWorker* worker
                                                     , QNetworkReply::NetworkError error_type
                                                     , QString& error_str)
{
    qDebug() << "ApolloCommunication::onGetServosModeSignalEFull" << error_type << error_str;
}

void ApolloCommunication::onGetMotionsListSignal(Swagger::SWGMotionsListResponse* summary)
{
    qDebug() << "ApolloCommunication::onGetMotionsListSignal" << summary->getCode() << summary->getMsg();
    if(summary->getCode() == 0) {
        auto musicList = summary->getData()->getMotions();
        QList<QString> musicNameList;
        for(Swagger::SWGMotionsInfo* info : *musicList) {
            QString* namePtr = info->getName();
            musicNameList.push_back(*namePtr);
            //TODO: 判断是否有音乐
        }
        qDebug() << "ApolloCommunication::onGetMotionsListSignal" << musicNameList;
        emit robotActionList(musicNameList);
    }
}

void ApolloCommunication::onGetMotionsListSignalEFull(Swagger::SWGHttpRequestWorker* worker
                                                      , QNetworkReply::NetworkError error_type
                                                      , QString& error_str)
{

}

void ApolloCommunication::onGetMediaMusicListSignal(Swagger::SWGMediaMusicListResponse* summary)
{
    int rc = summary->getCode();
    qDebug() << "ApolloCommunication::onGetMediaMusicListSignal" << rc;
    if(robotDisconnecting_) {
        robotDisconnecting_ = false;
        return;
    }
    if(rc == 0 && robotConnectionState_ == RobotConnecting) {
        changeRobotConnectionState(RobotConnected);
        if(connectionCheckingTimer_->isActive()) { connectionCheckingTimer_->stop(); }
        connectionCheckingTimer_->start(kConnectionCheckingInterval * 1000);
        connectionCheckingRetryTimes_ = kConnectionCheckingRetryTimes;
        getMotionsList();
    }
}

void ApolloCommunication::onGetMediaMusicListSignalEFull(Swagger::SWGHttpRequestWorker* worker
                                                         , QNetworkReply::NetworkError error_type
                                                         , QString& error_str)
{
    qDebug() << "ApolloCommunication::onGetMediaMusicListSignalEFull" << error_str;
    if(robotDisconnecting_) {
        robotDisconnecting_ = false;
        return;
    }
    if(connectionCheckingRetryTimes_ == 0) {
        changeRobotConnectionState(RobotUnconnected);
        //TODO: 断开后就不重连了，看产品定义
        disconnectRobot();
        return;
    }
    if(connectionCheckingRetryTimes_ > 0) {
        connectionCheckingRetryTimes_--;
        checkConnection();
    }
}

void ApolloCommunication::checkConnection()
{
    getMediaMusicList();
}

void ApolloCommunication::onConnectionCheckingTimeout()
{
    checkConnection();
}

void ApolloCommunication::onGetRobotServosTimeout()
{
    qDebug() << "ApolloCommunication::onGetRobotServosTimeout" << syncServoNames_;
    getServosAngles(syncServoNames_.toList());
}

void ApolloCommunication::onGetDevicesInfoTimeout()
{
    qDebug() << "ApolloCommunication::onGetDevicesInfoTimeout";
    getDevicesBattery();
    getDevicesVolume();
}

void ApolloCommunication::disableGetServos()
{
    if(getRobotServosTimer_->isActive()) {
        getRobotServosTimer_->stop();
    }
    syncServoNames_.clear();
}

void ApolloCommunication::removeSyncServoNames(QSet<QString> servoNames)
{
    syncServoNames_ -= servoNames;
    if(syncServoNames_.empty()) {
        if(getRobotServosTimer_->isActive()) {
            getRobotServosTimer_->stop();
        }
    }
}

void ApolloCommunication::getDevicesBattery()
{
    if(devicesApi_) {
        devicesApi_->getDevicesBattery();
    }
}
void ApolloCommunication::getDevicesVolume()
{
    if(devicesApi_) {
        devicesApi_->getDevicesVolume();
    }
}
void ApolloCommunication::putDevicesVolume(int volume)
{
    if(devicesApi_) {
        Swagger::SWGDevicesVolume swgVolume;
        swgVolume.setVolume(volume);
        devicesApi_->putDevicesVolume(swgVolume);
    }
}


void ApolloCommunication::onDevicesVolumeSignal(Swagger::SWGDevicesVolumeResponse* summary)
{
    qDebug() << "ApolloCommunication::onDevicesVolumeSignal" << summary->getCode() << summary->getData()->getVolume();
    if(summary->getCode() == 0) {
        emit volumeState(summary->getData()->getVolume());
    }
}
void ApolloCommunication::onDevicesVolumeSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str)
{
    qDebug() << "ApolloCommunication::onDevicesVolumeSignalEFull" << error_str;
}
void ApolloCommunication::onPutDevicesVolumeSignal(Swagger::SWGCommonResponse* summary)
{
    qDebug() << "ApolloCommunication::onPutDevicesVolumeSignal" << summary->getCode();
}
void ApolloCommunication::onPutDevicesVolumeSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str)
{
    qDebug() << "ApolloCommunication::onPutDevicesVolumeSignalEFull" << error_str;
}

void ApolloCommunication::onDeleteMotionsMusicSignal(Swagger::SWGCommonResponse* summary)
{
    qDebug() << "ApolloCommunication::onDeleteMotionsMusicSignal" << summary->getCode() << summary->getMsg();
}
void ApolloCommunication::onDeleteMotionsMusicSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str)
{
    qDebug() << "ApolloCommunication::onDeleteMotionsMusicSignalEFull" << error_str;
}
