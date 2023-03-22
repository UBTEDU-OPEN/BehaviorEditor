#ifndef ROBOTSEARCH_H
#define ROBOTSEARCH_H

#include "robotsearch_global.h"
#include"apollocommunication.h"
#include<QObject>
#include<QUdpSocket>
#include<QTimer>
#include<QList>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include<QDebug>
struct  RobotInfo
{

    QString cmd = "";
    QString name = "";
    QString ip = "";
    QString status = "";
    QString sn = "";

    bool operator==(const RobotInfo& other)const
    {
        if(this->name==other.name
                && this->cmd == other.cmd
                && this->ip == other.ip
                && this->status == other.status
                && this->sn == other.sn)
        {
            return true;
        }
        return false;
    }

};

class ROBOTSEARCH_EXPORT Robotsearch : public QObject
{
    Q_OBJECT
public:
    Robotsearch(QObject *parent = nullptr);
    ~Robotsearch();
    void startBroadcast(QString sn);
    void stopBroadcast();

    void startHeartbeatTimer();
    void stopHeartbeatTimer();

    void startRetryTimer();
    void stopRetryTimer();


    static Robotsearch* getInstance();
    void conectUdp();
    void setCurrentRobotInfo(RobotInfo robotInfo);
    void readAngleToRobot();
    void writeAngleToRobot(QMap<int,double> servos, int runtime);
    void readAngleToRobotResult(QString angle);
    void modifyServosToRobot(QString angle);
    void sendCmdToRobot(QString cmd);
    void udpConnectSuccess();

    void setApolloCommunication(ApolloCommunication* communication);
    void startConnectUdpTimer();
    void stopConnectUdpTimer();

    void startDisconnectUdpTimer();
    void stopDisconnectUdpTimer();

    void disconnectUdp();

signals:

    void searchRobotResult(QList<RobotInfo> *result);
    void searchTimeOut();


private slots:
    void broadcastData();
    void processBroadcastResponse();
    void jsonToRobotInfo(const QString &jsonStr);
    void onRobotConnectionStateChanged(int state);

private:
    int broadcastNo_;
    QUdpSocket *broadcastor_;
    QUdpSocket *broadcastor1_;
    QTimer *broadcastTimer_;
    QTimer *mHeartbeatTimer;
    QTimer *mRetryTimer;
    QTimer *mConnectUdpTimer;
    QTimer *mDisconnectUpdTimer;
    QList<RobotInfo> *robotInfos;
    QString mSn;
    RobotInfo mCurrentRobotInfo;
    QMap<int,double> mModifyServos;
    int mRuntime;
    ApolloCommunication *mCommunication;
    int mRetryTimes = 0;
    bool isConnected = false;
    bool isUdpConnected = false;
};

#endif // ROBOTSEARCH_H
