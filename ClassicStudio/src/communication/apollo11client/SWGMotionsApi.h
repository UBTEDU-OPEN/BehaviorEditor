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

#ifndef _SWG_SWGMotionsApi_H_
#define _SWG_SWGMotionsApi_H_

#include "SWGHttpRequest.h"

#include "SWGCommonResponse.h"
#include "SWGHttpRequest.h"
#include "SWGMotionsGaitOperation.h"
#include "SWGMotionsListResponse.h"
#include "SWGMotionsOperation.h"
#include "SWGMotionsStatusResponse.h"
#include "SWGName.h"
#include "SWGRuntimeResponse.h"
#include "SWGUserTime.h"

#include <QObject>

namespace Swagger {

class SWGMotionsApi: public QObject {
    Q_OBJECT

public:
    SWGMotionsApi();
    SWGMotionsApi(QString host, QString basePath);
    ~SWGMotionsApi();

    QString host;
    QString basePath;
    QMap<QString, QString> defaultHeaders;

    void deleteMotionsGait(SWGUserTime& body);
    void deleteMotionsMusic(SWGName& body);
    void getMotions();
    void getMotionsGait();
    void getMotionsList();
    void postMotions(SWGHttpRequestInputFileElement* file);
    void putMotions(SWGMotionsOperation& body);
    void putMotionsGait(SWGMotionsGaitOperation& body);
    
private:
    void deleteMotionsGaitCallback (SWGHttpRequestWorker * worker);
    void deleteMotionsMusicCallback (SWGHttpRequestWorker * worker);
    void getMotionsCallback (SWGHttpRequestWorker * worker);
    void getMotionsGaitCallback (SWGHttpRequestWorker * worker);
    void getMotionsListCallback (SWGHttpRequestWorker * worker);
    void postMotionsCallback (SWGHttpRequestWorker * worker);
    void putMotionsCallback (SWGHttpRequestWorker * worker);
    void putMotionsGaitCallback (SWGHttpRequestWorker * worker);
    
signals:
    void deleteMotionsGaitSignal(SWGCommonResponse* summary);
    void deleteMotionsMusicSignal(SWGCommonResponse* summary);
    void getMotionsSignal(SWGMotionsStatusResponse* summary);
    void getMotionsGaitSignal(SWGCommonResponse* summary);
    void getMotionsListSignal(SWGMotionsListResponse* summary);
    void postMotionsSignal(SWGCommonResponse* summary);
    void putMotionsSignal(SWGRuntimeResponse* summary);
    void putMotionsGaitSignal(SWGCommonResponse* summary);
    
    void deleteMotionsGaitSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteMotionsMusicSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getMotionsSignalE(SWGMotionsStatusResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getMotionsGaitSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getMotionsListSignalE(SWGMotionsListResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void postMotionsSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void putMotionsSignalE(SWGRuntimeResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void putMotionsGaitSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    
    void deleteMotionsGaitSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteMotionsMusicSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getMotionsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getMotionsGaitSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getMotionsListSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void postMotionsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void putMotionsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void putMotionsGaitSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    
};

}
#endif