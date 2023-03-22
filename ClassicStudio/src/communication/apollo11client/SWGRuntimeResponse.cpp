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


#include "SWGRuntimeResponse.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGRuntimeResponse::SWGRuntimeResponse(QString json) {
    init();
    this->fromJson(json);
}

SWGRuntimeResponse::SWGRuntimeResponse() {
    init();
}

SWGRuntimeResponse::~SWGRuntimeResponse() {
    this->cleanup();
}

void
SWGRuntimeResponse::init() {
    code = 0;
    m_code_isSet = false;
    data = new SWGTotalTime();
    m_data_isSet = false;
    msg = new QString("");
    m_msg_isSet = false;
}

void
SWGRuntimeResponse::cleanup() {

    if(data != nullptr) { 
        delete data;
    }
    if(msg != nullptr) { 
        delete msg;
    }
}

SWGRuntimeResponse*
SWGRuntimeResponse::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGRuntimeResponse::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&code, pJson["code"], "qint32", "");
    
    ::Swagger::setValue(&data, pJson["data"], "SWGTotalTime", "SWGTotalTime");
    
    ::Swagger::setValue(&msg, pJson["msg"], "QString", "QString");
    
}

QString
SWGRuntimeResponse::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGRuntimeResponse::asJsonObject() {
    QJsonObject obj;
    if(m_code_isSet){
        obj.insert("code", QJsonValue(code));
    }
    if((data != nullptr) && (data->isSet())){
        toJsonValue(QString("data"), data, obj, QString("SWGTotalTime"));
    }
    if(msg != nullptr && *msg != QString("")){
        toJsonValue(QString("msg"), msg, obj, QString("QString"));
    }

    return obj;
}

qint32
SWGRuntimeResponse::getCode() {
    return code;
}
void
SWGRuntimeResponse::setCode(qint32 code) {
    this->code = code;
    this->m_code_isSet = true;
}

SWGTotalTime*
SWGRuntimeResponse::getData() {
    return data;
}
void
SWGRuntimeResponse::setData(SWGTotalTime* data) {
    this->data = data;
    this->m_data_isSet = true;
}

QString*
SWGRuntimeResponse::getMsg() {
    return msg;
}
void
SWGRuntimeResponse::setMsg(QString* msg) {
    this->msg = msg;
    this->m_msg_isSet = true;
}


bool
SWGRuntimeResponse::isSet(){
    bool isObjectUpdated = false;
    do{
        if(m_code_isSet){ isObjectUpdated = true; break;}
        if(data != nullptr && data->isSet()){ isObjectUpdated = true; break;}
        if(msg != nullptr && *msg != QString("")){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

