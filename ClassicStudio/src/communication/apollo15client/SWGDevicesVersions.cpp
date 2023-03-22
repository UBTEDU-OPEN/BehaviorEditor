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


#include "SWGDevicesVersions.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGDevicesVersions::SWGDevicesVersions(QString json) {
    init();
    this->fromJson(json);
}

SWGDevicesVersions::SWGDevicesVersions() {
    init();
}

SWGDevicesVersions::~SWGDevicesVersions() {
    this->cleanup();
}

void
SWGDevicesVersions::init() {
    core = new QString("");
    m_core_isSet = false;
    servo = new QString("");
    m_servo_isSet = false;
    sn = new QString("");
    m_sn_isSet = false;
}

void
SWGDevicesVersions::cleanup() {
    if(core != nullptr) { 
        delete core;
    }
    if(servo != nullptr) { 
        delete servo;
    }
    if(sn != nullptr) { 
        delete sn;
    }
}

SWGDevicesVersions*
SWGDevicesVersions::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGDevicesVersions::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&core, pJson["core"], "QString", "QString");
    
    ::Swagger::setValue(&servo, pJson["servo"], "QString", "QString");
    
    ::Swagger::setValue(&sn, pJson["sn"], "QString", "QString");
    
}

QString
SWGDevicesVersions::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGDevicesVersions::asJsonObject() {
    QJsonObject obj;
    if(core != nullptr && *core != QString("")){
        toJsonValue(QString("core"), core, obj, QString("QString"));
    }
    if(servo != nullptr && *servo != QString("")){
        toJsonValue(QString("servo"), servo, obj, QString("QString"));
    }
    if(sn != nullptr && *sn != QString("")){
        toJsonValue(QString("sn"), sn, obj, QString("QString"));
    }

    return obj;
}

QString*
SWGDevicesVersions::getCore() {
    return core;
}
void
SWGDevicesVersions::setCore(QString* core) {
    this->core = core;
    this->m_core_isSet = true;
}

QString*
SWGDevicesVersions::getServo() {
    return servo;
}
void
SWGDevicesVersions::setServo(QString* servo) {
    this->servo = servo;
    this->m_servo_isSet = true;
}

QString*
SWGDevicesVersions::getSn() {
    return sn;
}
void
SWGDevicesVersions::setSn(QString* sn) {
    this->sn = sn;
    this->m_sn_isSet = true;
}


bool
SWGDevicesVersions::isSet(){
    bool isObjectUpdated = false;
    do{
        if(core != nullptr && *core != QString("")){ isObjectUpdated = true; break;}
        if(servo != nullptr && *servo != QString("")){ isObjectUpdated = true; break;}
        if(sn != nullptr && *sn != QString("")){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

