#ifndef APOLLOCOMMUNICATION_H
#define APOLLOCOMMUNICATION_H

#include "communication_global.h"
#include <QObject>
#include <QTimer>

#if defined(YANSHEE1_5)
#include "apollo15client/SWGDevicesApi.h"
#include "apollo15client/SWGMotionsApi.h"
#include "apollo15client/SWGServosApi.h"
#include "apollo15client/SWGMediaApi.h"
#else
#include "apollo11client/SWGDevicesApi.h"
#include "apollo11client/SWGMotionsApi.h"
#include "apollo11client/SWGServosApi.h"
#include "apollo11client/SWGMediaApi.h"
#endif

class COMMUNICATION_EXPORT ApolloCommunication : public QObject
{
    Q_OBJECT
public:
    enum RobotConnectionState{
        RobotUnconnected = 0,
        RobotConnecting = 1,
        RobotConnected = 2
    };

    explicit ApolloCommunication(QObject* parent = nullptr);
    void postFileToRobot(const QString& filePath);
    void connectRobot(const QString& addr);
    void disconnectRobot();
    void putCommand(const QString& name, const QString& dirction, int repeatCount, const QString& speed, const QString& operation);
    void putServosAngles(const QMap<QString,double>& servos,int runtime = 500);
    void getServosAngles(const QList<QString>& servoNames);
    void putServosMode(const QList<QString>& servoNames, bool isLock);
    void getServosMode(const QList<QString>& servoNames);
    void getMotionsList();
    void getMediaMusicList();
    void deleteMotions(const QString&);
    RobotConnectionState robotConnectionState() { return robotConnectionState_; }
    void disableGetServos();
    void getDevicesBattery();
    void getDevicesVolume();
    void putDevicesVolume(int);

private slots:
    void onDevicesBatterySignal(Swagger::SWGDevicesBatteryResponse* summary);
    void onDevicesBatterySignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onDevicesVolumeSignal(Swagger::SWGDevicesVolumeResponse* summary);
    void onDevicesVolumeSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onPutDevicesVolumeSignal(Swagger::SWGCommonResponse* summary);
    void onPutDevicesVolumeSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onPostMotionsSignal(Swagger::SWGCommonResponse* summary);
    void onPostMotionsSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onPutMotionsSignal(Swagger::SWGRuntimeResponse* summary);
    void onPutMotionsSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onPutServosAnglesSignal(Swagger::SWGServosResultResponse* summary);
    void onPutServosAnglesSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onGetServosAnglesSignal(Swagger::SWGServosAnglesResponse* summary);
    void onGetServosAnglesSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onPutServosModeSignal(Swagger::SWGServosResultResponse* summary);
    void onPutServosModeSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onGetServosModeSignal(Swagger::SWGServosModeResponse* summary);
    void onGetServosModeSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onGetMotionsListSignal(Swagger::SWGMotionsListResponse* summary);
    void onGetMotionsListSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onGetMediaMusicListSignal(Swagger::SWGMediaMusicListResponse* summary);
    void onGetMediaMusicListSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onDeleteMotionsMusicSignal(Swagger::SWGCommonResponse* summary);
    void onDeleteMotionsMusicSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onPostMusicSignal(Swagger::SWGCommonResponse* summary);
    void onPostMusicSignalEFull(Swagger::SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void onConnectionCheckingTimeout();
    void onGetRobotServosTimeout();
    void onGetDevicesInfoTimeout();

signals:
    void robotConnectionStateChanged(int);
    void robotServosAngles(const QMap<QString,int>& servosAngles);
    void robotActionList(const QList<QString>& actionList);
    void sendFileResult(bool);
    void batteryState(int inCharging, int percent);
    void volumeState(int volume);

private:   
    void changeRobotConnectionState(RobotConnectionState);
    void checkConnection();
    void removeSyncServoNames(QSet<QString> servoNames);

private:
    QString currentRobotAddr_;
    RobotConnectionState robotConnectionState_;
    Swagger::SWGDevicesApi* devicesApi_;
    Swagger::SWGMotionsApi* motionsApi_;
    Swagger::SWGServosApi* servosApi_;
    Swagger::SWGMediaApi* mediaApi_;
    QTimer* connectionCheckingTimer_;
    QTimer* getRobotServosTimer_;
    QTimer* getDevicesInfoTimer_;
    QSet<QString> syncServoNames_;
    bool robotDisconnecting_;
    int connectionCheckingRetryTimes_;
};

#endif // APOLLOCOMMUNICATION_H
