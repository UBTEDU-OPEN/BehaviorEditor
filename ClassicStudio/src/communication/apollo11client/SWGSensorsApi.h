/**
 * Yanshee RESTful API
 *  ## 概述 Yanshee RESTful APIs是由使用[swagger-codegen](https://github.com/swagger-api/swagger-codegen)基于 [OpenAPI-Spec](https://github.com/swagger-api/swagger-core/wiki)的工程。 所有的API由Flask的[Connexion](https://github.com/zalando/connexion)来解释。 Yanshee-ROS中的apollo提供所有的RESTful APIs服务。 同时Yanshee RESTful APIs提供英文和中文两个版本： - [英文版本](https://app.swaggerhub.com/apis-docs/UBTEDU/apollo_en/1.0.0) - [中文版本](https://app.swaggerhub.com/apis-docs/UBTEDU/apollo_cn/1.0.0)  ## 依赖 Python 2.7 or 3.4+  
 *
 * OpenAPI spec version: 1.0.1
 * Contact: swenggroup@ubtrobot.com
 *
 * NOTE: This class is auto generated by the swagger code generator program.
 * https://github.com/swagger-api/swagger-codegen.git
 * Do not edit the class manually.
 */

#ifndef _SWG_SWGSensorsApi_H_
#define _SWG_SWGSensorsApi_H_

#include "SWGHttpRequest.h"

#include "SWGCommonResponse.h"
#include "SWGSensorsEnvironmentValueResponse.h"
#include "SWGSensorsGyroValueResponse.h"
#include "SWGSensorsInfraredValueResponse.h"
#include "SWGSensorsListResponse.h"
#include "SWGSensorsOperationRequest.h"
#include "SWGSensorsPressureValueResponse.h"
#include "SWGSensorsTouchValueResponse.h"
#include "SWGSensorsUltrasonicValueResponse.h"

#include <QObject>

namespace Swagger {

class SWGSensorsApi: public QObject {
    Q_OBJECT

public:
    SWGSensorsApi();
    SWGSensorsApi(QString host, QString basePath);
    ~SWGSensorsApi();

    QString host;
    QString basePath;
    QMap<QString, QString> defaultHeaders;

    void getSensorsEnvironment(QList<qint32>* slot);
    void getSensorsGyro();
    void getSensorsInfrared(QList<qint32>* id, QList<qint32>* slot);
    void getSensorsList();
    void getSensorsPressure(QList<qint32>* id, QList<qint32>* slot);
    void getSensorsTouch(QList<qint32>* id, QList<qint32>* slot);
    void getSensorsUltrasonic(QList<qint32>* id, QList<qint32>* slot);
    void putSensors(SWGSensorsOperationRequest& body);
    
private:
    void getSensorsEnvironmentCallback (SWGHttpRequestWorker * worker);
    void getSensorsGyroCallback (SWGHttpRequestWorker * worker);
    void getSensorsInfraredCallback (SWGHttpRequestWorker * worker);
    void getSensorsListCallback (SWGHttpRequestWorker * worker);
    void getSensorsPressureCallback (SWGHttpRequestWorker * worker);
    void getSensorsTouchCallback (SWGHttpRequestWorker * worker);
    void getSensorsUltrasonicCallback (SWGHttpRequestWorker * worker);
    void putSensorsCallback (SWGHttpRequestWorker * worker);
    
signals:
    void getSensorsEnvironmentSignal(SWGSensorsEnvironmentValueResponse* summary);
    void getSensorsGyroSignal(SWGSensorsGyroValueResponse* summary);
    void getSensorsInfraredSignal(SWGSensorsInfraredValueResponse* summary);
    void getSensorsListSignal(SWGSensorsListResponse* summary);
    void getSensorsPressureSignal(SWGSensorsPressureValueResponse* summary);
    void getSensorsTouchSignal(SWGSensorsTouchValueResponse* summary);
    void getSensorsUltrasonicSignal(SWGSensorsUltrasonicValueResponse* summary);
    void putSensorsSignal(SWGCommonResponse* summary);
    
    void getSensorsEnvironmentSignalE(SWGSensorsEnvironmentValueResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsGyroSignalE(SWGSensorsGyroValueResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsInfraredSignalE(SWGSensorsInfraredValueResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsListSignalE(SWGSensorsListResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsPressureSignalE(SWGSensorsPressureValueResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsTouchSignalE(SWGSensorsTouchValueResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsUltrasonicSignalE(SWGSensorsUltrasonicValueResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void putSensorsSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    
    void getSensorsEnvironmentSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsGyroSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsInfraredSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsListSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsPressureSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsTouchSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getSensorsUltrasonicSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void putSensorsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    
};

}
#endif
