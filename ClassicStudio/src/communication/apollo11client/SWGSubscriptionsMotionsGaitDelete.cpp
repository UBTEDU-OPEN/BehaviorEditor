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


#include "SWGSubscriptionsMotionsGaitDelete.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGSubscriptionsMotionsGaitDelete::SWGSubscriptionsMotionsGaitDelete(QString json) {
    init();
    this->fromJson(json);
}

SWGSubscriptionsMotionsGaitDelete::SWGSubscriptionsMotionsGaitDelete() {
    init();
}

SWGSubscriptionsMotionsGaitDelete::~SWGSubscriptionsMotionsGaitDelete() {
    this->cleanup();
}

void
SWGSubscriptionsMotionsGaitDelete::init() {
    url = new QString("");
    m_url_isSet = false;
}

void
SWGSubscriptionsMotionsGaitDelete::cleanup() {
    if(url != nullptr) { 
        delete url;
    }
}

SWGSubscriptionsMotionsGaitDelete*
SWGSubscriptionsMotionsGaitDelete::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGSubscriptionsMotionsGaitDelete::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&url, pJson["url"], "QString", "QString");
    
}

QString
SWGSubscriptionsMotionsGaitDelete::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGSubscriptionsMotionsGaitDelete::asJsonObject() {
    QJsonObject obj;
    if(url != nullptr && *url != QString("")){
        toJsonValue(QString("url"), url, obj, QString("QString"));
    }

    return obj;
}

QString*
SWGSubscriptionsMotionsGaitDelete::getUrl() {
    return url;
}
void
SWGSubscriptionsMotionsGaitDelete::setUrl(QString* url) {
    this->url = url;
    this->m_url_isSet = true;
}


bool
SWGSubscriptionsMotionsGaitDelete::isSet(){
    bool isObjectUpdated = false;
    do{
        if(url != nullptr && *url != QString("")){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

