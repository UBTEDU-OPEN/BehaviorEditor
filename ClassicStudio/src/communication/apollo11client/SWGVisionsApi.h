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

#ifndef _SWG_SWGVisionsApi_H_
#define _SWG_SWGVisionsApi_H_

#include "SWGHttpRequest.h"

#include <QString>
#include "SWGCommonResponse.h"
#include "SWGHttpRequest.h"
#include "SWGName.h"
#include "SWGVisionsDeleteTags.h"
#include "SWGVisionsGetResponse.h"
#include "SWGVisionsPhoto.h"
#include "SWGVisionsPhotoListResponse.h"
#include "SWGVisionsPhotoResponse.h"
#include "SWGVisionsPutResponse.h"
#include "SWGVisionsPutTags.h"
#include "SWGVisionsStream.h"
#include "SWGVisionsTagsResponse.h"
#include "SWGVisionsTask.h"

#include <QObject>

namespace Swagger {

class SWGVisionsApi: public QObject {
    Q_OBJECT

public:
    SWGVisionsApi();
    SWGVisionsApi(QString host, QString basePath);
    ~SWGVisionsApi();

    QString host;
    QString basePath;
    QMap<QString, QString> defaultHeaders;

    void deleteVisionPhoto(SWGName& body);
    void deleteVisionPhotoSamples(SWGName& body);
    void deleteVisionsStreams();
    void deleteVisionsTags(SWGVisionsDeleteTags& body);
    void getPhotoSamples();
    void getVision(QString* option, QString* type);
    void getVisionsPhotos(QString* body);
    void getVisionsPhotosLists();
    void getVisionsTags();
    void postVisionPhoto(SWGVisionsPhoto& body);
    void postVisionsPhotoSamples(SWGHttpRequestInputFileElement* file);
    void postVisionsStreams(SWGVisionsStream& body);
    void putVisions(SWGVisionsTask& body);
    void putVisionsTags(SWGVisionsPutTags& body);
    
private:
    void deleteVisionPhotoCallback (SWGHttpRequestWorker * worker);
    void deleteVisionPhotoSamplesCallback (SWGHttpRequestWorker * worker);
    void deleteVisionsStreamsCallback (SWGHttpRequestWorker * worker);
    void deleteVisionsTagsCallback (SWGHttpRequestWorker * worker);
    void getPhotoSamplesCallback (SWGHttpRequestWorker * worker);
    void getVisionCallback (SWGHttpRequestWorker * worker);
    void getVisionsPhotosCallback (SWGHttpRequestWorker * worker);
    void getVisionsPhotosListsCallback (SWGHttpRequestWorker * worker);
    void getVisionsTagsCallback (SWGHttpRequestWorker * worker);
    void postVisionPhotoCallback (SWGHttpRequestWorker * worker);
    void postVisionsPhotoSamplesCallback (SWGHttpRequestWorker * worker);
    void postVisionsStreamsCallback (SWGHttpRequestWorker * worker);
    void putVisionsCallback (SWGHttpRequestWorker * worker);
    void putVisionsTagsCallback (SWGHttpRequestWorker * worker);
    
signals:
    void deleteVisionPhotoSignal(SWGCommonResponse* summary);
    void deleteVisionPhotoSamplesSignal(SWGCommonResponse* summary);
    void deleteVisionsStreamsSignal(SWGCommonResponse* summary);
    void deleteVisionsTagsSignal(SWGCommonResponse* summary);
    void getPhotoSamplesSignal(SWGVisionsPhotoListResponse* summary);
    void getVisionSignal(SWGVisionsGetResponse* summary);
    void getVisionsPhotosSignal();
    void getVisionsPhotosListsSignal(SWGVisionsPhotoListResponse* summary);
    void getVisionsTagsSignal(SWGVisionsTagsResponse* summary);
    void postVisionPhotoSignal(SWGVisionsPhotoResponse* summary);
    void postVisionsPhotoSamplesSignal(SWGCommonResponse* summary);
    void postVisionsStreamsSignal(SWGCommonResponse* summary);
    void putVisionsSignal(SWGVisionsPutResponse* summary);
    void putVisionsTagsSignal(SWGCommonResponse* summary);
    
    void deleteVisionPhotoSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVisionPhotoSamplesSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVisionsStreamsSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVisionsTagsSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getPhotoSamplesSignalE(SWGVisionsPhotoListResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVisionSignalE(SWGVisionsGetResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVisionsPhotosSignalE(QNetworkReply::NetworkError error_type, QString& error_str);
    void getVisionsPhotosListsSignalE(SWGVisionsPhotoListResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVisionsTagsSignalE(SWGVisionsTagsResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void postVisionPhotoSignalE(SWGVisionsPhotoResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void postVisionsPhotoSamplesSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void postVisionsStreamsSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVisionsSignalE(SWGVisionsPutResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVisionsTagsSignalE(SWGCommonResponse* summary, QNetworkReply::NetworkError error_type, QString& error_str);
    
    void deleteVisionPhotoSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVisionPhotoSamplesSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVisionsStreamsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void deleteVisionsTagsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getPhotoSamplesSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVisionSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVisionsPhotosSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVisionsPhotosListsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void getVisionsTagsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void postVisionPhotoSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void postVisionsPhotoSamplesSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void postVisionsStreamsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVisionsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    void putVisionsTagsSignalEFull(SWGHttpRequestWorker* worker, QNetworkReply::NetworkError error_type, QString& error_str);
    
};

}
#endif
