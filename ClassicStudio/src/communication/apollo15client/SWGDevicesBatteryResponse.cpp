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


#include "SWGDevicesBatteryResponse.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGDevicesBatteryResponse::SWGDevicesBatteryResponse(QString json) {
    init();
    this->fromJson(json);
}

SWGDevicesBatteryResponse::SWGDevicesBatteryResponse() {
    init();
}

SWGDevicesBatteryResponse::~SWGDevicesBatteryResponse() {
    this->cleanup();
}

void
SWGDevicesBatteryResponse::init() {
    code = 0;
    m_code_isSet = false;
    data = new SWGDevicesBattery();
    m_data_isSet = false;
    msg = new QString("");
    m_msg_isSet = false;
}

void
SWGDevicesBatteryResponse::cleanup() {

    if(data != nullptr) { 
        delete data;
    }
    if(msg != nullptr) { 
        delete msg;
    }
}

SWGDevicesBatteryResponse*
SWGDevicesBatteryResponse::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGDevicesBatteryResponse::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&code, pJson["code"], "qint32", "");
    
    ::Swagger::setValue(&data, pJson["data"], "SWGDevicesBattery", "SWGDevicesBattery");
    
    ::Swagger::setValue(&msg, pJson["msg"], "QString", "QString");
    
}

QString
SWGDevicesBatteryResponse::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGDevicesBatteryResponse::asJsonObject() {
    QJsonObject obj;
    if(m_code_isSet){
        obj.insert("code", QJsonValue(code));
    }
    if((data != nullptr) && (data->isSet())){
        toJsonValue(QString("data"), data, obj, QString("SWGDevicesBattery"));
    }
    if(msg != nullptr && *msg != QString("")){
        toJsonValue(QString("msg"), msg, obj, QString("QString"));
    }

    return obj;
}

qint32
SWGDevicesBatteryResponse::getCode() {
    return code;
}
void
SWGDevicesBatteryResponse::setCode(qint32 code) {
    this->code = code;
    this->m_code_isSet = true;
}

SWGDevicesBattery*
SWGDevicesBatteryResponse::getData() {
    return data;
}
void
SWGDevicesBatteryResponse::setData(SWGDevicesBattery* data) {
    this->data = data;
    this->m_data_isSet = true;
}

QString*
SWGDevicesBatteryResponse::getMsg() {
    return msg;
}
void
SWGDevicesBatteryResponse::setMsg(QString* msg) {
    this->msg = msg;
    this->m_msg_isSet = true;
}


bool
SWGDevicesBatteryResponse::isSet(){
    bool isObjectUpdated = false;
    do{
        if(m_code_isSet){ isObjectUpdated = true; break;}
        if(data != nullptr && data->isSet()){ isObjectUpdated = true; break;}
        if(msg != nullptr && *msg != QString("")){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

