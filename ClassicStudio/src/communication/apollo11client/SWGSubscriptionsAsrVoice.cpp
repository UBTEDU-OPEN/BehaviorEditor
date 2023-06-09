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


#include "SWGSubscriptionsAsrVoice.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGSubscriptionsAsrVoice::SWGSubscriptionsAsrVoice(QString json) {
    init();
    this->fromJson(json);
}

SWGSubscriptionsAsrVoice::SWGSubscriptionsAsrVoice() {
    init();
}

SWGSubscriptionsAsrVoice::~SWGSubscriptionsAsrVoice() {
    this->cleanup();
}

void
SWGSubscriptionsAsrVoice::init() {
    url = new QString("");
    m_url_isSet = false;
    timeout = 0;
    m_timeout_isSet = false;
}

void
SWGSubscriptionsAsrVoice::cleanup() {
    if(url != nullptr) { 
        delete url;
    }

}

SWGSubscriptionsAsrVoice*
SWGSubscriptionsAsrVoice::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGSubscriptionsAsrVoice::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&url, pJson["url"], "QString", "QString");
    
    ::Swagger::setValue(&timeout, pJson["timeout"], "qint32", "");
    
}

QString
SWGSubscriptionsAsrVoice::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGSubscriptionsAsrVoice::asJsonObject() {
    QJsonObject obj;
    if(url != nullptr && *url != QString("")){
        toJsonValue(QString("url"), url, obj, QString("QString"));
    }
    if(m_timeout_isSet){
        obj.insert("timeout", QJsonValue(timeout));
    }

    return obj;
}

QString*
SWGSubscriptionsAsrVoice::getUrl() {
    return url;
}
void
SWGSubscriptionsAsrVoice::setUrl(QString* url) {
    this->url = url;
    this->m_url_isSet = true;
}

qint32
SWGSubscriptionsAsrVoice::getTimeout() {
    return timeout;
}
void
SWGSubscriptionsAsrVoice::setTimeout(qint32 timeout) {
    this->timeout = timeout;
    this->m_timeout_isSet = true;
}


bool
SWGSubscriptionsAsrVoice::isSet(){
    bool isObjectUpdated = false;
    do{
        if(url != nullptr && *url != QString("")){ isObjectUpdated = true; break;}
        if(m_timeout_isSet){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

