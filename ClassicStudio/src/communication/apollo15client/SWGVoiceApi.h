/**
 * Yanshee RESTful API
 *  ## 概述 Yanshee RESTful APIs是由使用[swagger-codegen](https://github.com/swagger-api/swagger-codegen)基于 [OpenAPI-Spec](https://github.com/swagger-api/swagger-core/wiki)的工程。 所有的API由Flask的[Connexion](https://github.com/zalando/connexion)来解释。 Yanshee-ROS中的apollo提供所有的RESTful APIs服务。 同时Yanshee RESTful APIs提供英文和中文两个版本： - [英文版本](https://app.swaggerhub.com/apis-docs/UBTEDU/apollo_en/1.0.0) - [中文版本](https://app.swaggerhub.com/apis-docs/UBTEDU/apollo_cn/1.0.0)  ## 依赖 Python 2.7 or 3.4+  
 *
 * OpenAPI spec version: 1.0.0
 * Contact: swenggroup@ubtrobot.com
 *
 * NOTE: This class is auto generated by the swagger code generator program.
 * https://github.com/swagger-api/swagger-codegen.git
 * Do not edit the class manually.
 */

#ifndef _SWG_SWGVoiceApi_H_
#define _SWG_SWGVoiceApi_H_

#include "SWGHttpRequest.h"

#include <QString>
#include "SWGCommonResponse.h"
#include "SWGVoiceAsrOption.h"
#include "SWGVoiceDeleteOfflineSyntax.h"
#include "SWGVoiceGetOfflineSyntaxGrammarsResponse.h"
#include "SWGVoiceGetOfflineSyntaxResponse.h"
#include "SWGVoiceGetResponse.h"
#include "SWGVoiceIatRequest.h"
#include "SWGVoicePostOfflineSyntax.h"
#include "SWGVoicePutOfflineSyntax.h"
#include "SWGVoiceTTSStr.h"

#include <QObject>

namespace Swagger {

class SWGVoiceApi: public QObject {
    Q_OBJECT

public:
    SWGVoiceApi();
    SWGVoiceApi(QString host, QString basePath);
    ~SWGVoiceApi();

    QString host;
    QString basePath;
    QMap<QString, QString> defaultHeaders;

    void deleteVoiceAsr();
    void deleteVoiceIat();
    void deleteVoiceOfflineSyntax(SWGVoiceDeleteOfflineSyntax& body);
    void deleteVoiceTts();
    void getVoiceAsr();
    void getVoiceIat();
    void getVoiceOfflineSyntax(QString* body);
    void getVoiceOfflineSyntaxGrammars();
    void getVoiceTts(qint64 timestamp);
    void postVoiceOfflineSyntax(SWGVoicePostOfflineSyntax& body);
    void putVoiceAsr(SWGVoiceAsrOption& body);
    void putVoiceIat(SWGVoiceIatRequest& body);
    void putVoiceOfflineSyntax(SWGVoicePutOfflineSyntax& body);
    void putVoiceTts(SWGVoiceTTSStr& body);
    
private:
    void deleteVoiceAsrCallback (SWGHttpRequestWorker * worker);
    void deleteVoiceIatCallback (SWGHttpRequestWorker * worker);
    void deleteVoiceOfflineSyntaxCallback (SWGHttpRequestWorker * worker);
    void deleteVoiceTtsCallback (SWGHttpRequestWorker * worker);
    void getVoiceAsrCallback (SWGHttpRequestWorker * worker);
    void getVoiceIatCallback (SWGHttpRequestWorker * worker);
    void getVoiceOfflineSyntaxCallback (SWGHttpRequestWorker * worker);
    void getVoiceOfflineSyntaxGrammarsCallback (SWGHttpRequestWorker * worker);
    void getVoiceTtsCallback (SWGHttpRequestWorker * worker);
    void postVoiceOfflineSyntaxCallback (SWGHttpRequestWorker * worker);
    void putVoiceAsrCallback (SWGHttpRequestWorker * worker);
    void putVoiceIatCallback (SWGHttpRequestWorker * worker);
    void putVoiceOfflineSyntaxCallback (SWGHttpRequestWorker * worker);
    void putVoiceTtsCallback (SWGHttpRequestWorker * worker);
    
signals:
    void deleteVoiceAsrSignal(SWGCommonResponse* summary);
    void deleteVoiceIatSignal(SWGCommonResponse* summary);
    void deleteVoiceOfflineSyntaxSignal(SWGCommonResponse* summary);
    void deleteVoiceTtsSignal(SWGCommonResponse* summary);
    void getVoiceAsrSignal(SWGVoiceGetResponse* summary);
    void getVoiceIatSignal(SWGVoiceGetResponse* summary);
    void getVoiceOfflineSyntaxSignal(SWGVoiceGetOfflineSyntaxResponse* summary);
    void getVoiceOfflineSyntaxGrammarsSignal(SWGVoiceGetOfflineSyntaxGrammarsResponse* summary);
    void getVoiceTtsSignal(SWGVoiceGetResponse* summary);
    void postVoiceOfflineSyntaxSignal(SWGCommonResponse* summary);
    void putVoiceAsrSignal(SWGCommonResponse* summary);
    void putVoiceIatSignal(SWGCommonResponse* summary);
    void putVoiceOfflineSyntaxSignal(SWGCommonResponse* summary);
    void putVoiceTtsSignal(SWGCommonResponse* summary);
    
    void deleteVoiceAsrSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVoiceIatSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVoiceOfflineSyntaxSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVoiceTtsSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVoiceAsrSignalE(SWGVoiceGetResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVoiceIatSignalE(SWGVoiceGetResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVoiceOfflineSyntaxSignalE(SWGVoiceGetOfflineSyntaxResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVoiceOfflineSyntaxGrammarsSignalE(SWGVoiceGetOfflineSyntaxGrammarsResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVoiceTtsSignalE(SWGVoiceGetResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void postVoiceOfflineSyntaxSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVoiceAsrSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVoiceIatSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVoiceOfflineSyntaxSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVoiceTtsSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    
    void deleteVoiceAsrSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVoiceIatSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVoiceOfflineSyntaxSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVoiceTtsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVoiceAsrSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVoiceIatSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVoiceOfflineSyntaxSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVoiceOfflineSyntaxGrammarsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVoiceTtsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void postVoiceOfflineSyntaxSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVoiceAsrSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVoiceIatSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVoiceOfflineSyntaxSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVoiceTtsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    
};

}
#endif
