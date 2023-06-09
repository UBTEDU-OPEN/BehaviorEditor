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


#include "SWGDevicesLEDResponse.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGDevicesLEDResponse::SWGDevicesLEDResponse(QString json) {
    init();
    this->fromJson(json);
}

SWGDevicesLEDResponse::SWGDevicesLEDResponse() {
    init();
}

SWGDevicesLEDResponse::~SWGDevicesLEDResponse() {
    this->cleanup();
}

void
SWGDevicesLEDResponse::init() {
    code = 0;
    m_code_isSet = false;
    data = new QList<SWGDevicesLED*>();
    m_data_isSet = false;
    msg = new QString("");
    m_msg_isSet = false;
}

void
SWGDevicesLEDResponse::cleanup() {

    if(data != nullptr) { 
        auto arr = data;
        for(auto o: *arr) { 
            delete o;
        }
        delete data;
    }
    if(msg != nullptr) { 
        delete msg;
    }
}

SWGDevicesLEDResponse*
SWGDevicesLEDResponse::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGDevicesLEDResponse::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&code, pJson["code"], "qint32", "");
    
    
    ::Swagger::setValue(&data, pJson["data"], "QList", "SWGDevicesLED");
    ::Swagger::setValue(&msg, pJson["msg"], "QString", "QString");
    
}

QString
SWGDevicesLEDResponse::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGDevicesLEDResponse::asJsonObject() {
    QJsonObject obj;
    if(m_code_isSet){
        obj.insert("code", QJsonValue(code));
    }
    if(data->size() > 0){
        toJsonArray((QList<void*>*)data, obj, "data", "SWGDevicesLED");
    }
    if(msg != nullptr && *msg != QString("")){
        toJsonValue(QString("msg"), msg, obj, QString("QString"));
    }

    return obj;
}

qint32
SWGDevicesLEDResponse::getCode() {
    return code;
}
void
SWGDevicesLEDResponse::setCode(qint32 code) {
    this->code = code;
    this->m_code_isSet = true;
}

QList<SWGDevicesLED*>*
SWGDevicesLEDResponse::getData() {
    return data;
}
void
SWGDevicesLEDResponse::setData(QList<SWGDevicesLED*>* data) {
    this->data = data;
    this->m_data_isSet = true;
}

QString*
SWGDevicesLEDResponse::getMsg() {
    return msg;
}
void
SWGDevicesLEDResponse::setMsg(QString* msg) {
    this->msg = msg;
    this->m_msg_isSet = true;
}


bool
SWGDevicesLEDResponse::isSet(){
    bool isObjectUpdated = false;
    do{
        if(m_code_isSet){ isObjectUpdated = true; break;}
        if(data->size() > 0){ isObjectUpdated = true; break;}
        if(msg != nullptr && *msg != QString("")){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

