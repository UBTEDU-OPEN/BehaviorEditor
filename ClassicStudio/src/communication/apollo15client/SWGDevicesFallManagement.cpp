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


#include "SWGDevicesFallManagement.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGDevicesFallManagement::SWGDevicesFallManagement(QString json) {
    init();
    this->fromJson(json);
}

SWGDevicesFallManagement::SWGDevicesFallManagement() {
    init();
}

SWGDevicesFallManagement::~SWGDevicesFallManagement() {
    this->cleanup();
}

void
SWGDevicesFallManagement::init() {
    enable = false;
    m_enable_isSet = false;
}

void
SWGDevicesFallManagement::cleanup() {

}

SWGDevicesFallManagement*
SWGDevicesFallManagement::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGDevicesFallManagement::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&enable, pJson["enable"], "bool", "");
    
}

QString
SWGDevicesFallManagement::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGDevicesFallManagement::asJsonObject() {
    QJsonObject obj;
    if(m_enable_isSet){
        obj.insert("enable", QJsonValue(enable));
    }

    return obj;
}

bool
SWGDevicesFallManagement::isEnable() {
    return enable;
}
void
SWGDevicesFallManagement::setEnable(bool enable) {
    this->enable = enable;
    this->m_enable_isSet = true;
}


bool
SWGDevicesFallManagement::isSet(){
    bool isObjectUpdated = false;
    do{
        if(m_enable_isSet){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

