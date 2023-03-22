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


#include "SWGUserTime.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGUserTime::SWGUserTime(QString json) {
    init();
    this->fromJson(json);
}

SWGUserTime::SWGUserTime() {
    init();
}

SWGUserTime::~SWGUserTime() {
    this->cleanup();
}

void
SWGUserTime::init() {
    timestamp = 0;
    m_timestamp_isSet = false;
}

void
SWGUserTime::cleanup() {

}

SWGUserTime*
SWGUserTime::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGUserTime::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&timestamp, pJson["timestamp"], "qint32", "");
    
}

QString
SWGUserTime::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGUserTime::asJsonObject() {
    QJsonObject obj;
    if(m_timestamp_isSet){
        obj.insert("timestamp", QJsonValue(timestamp));
    }

    return obj;
}

qint32
SWGUserTime::getTimestamp() {
    return timestamp;
}
void
SWGUserTime::setTimestamp(qint32 timestamp) {
    this->timestamp = timestamp;
    this->m_timestamp_isSet = true;
}


bool
SWGUserTime::isSet(){
    bool isObjectUpdated = false;
    do{
        if(m_timestamp_isSet){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

