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


#include "SWGSensorsOperationRequest.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGSensorsOperationRequest::SWGSensorsOperationRequest(QString json) {
    init();
    this->fromJson(json);
}

SWGSensorsOperationRequest::SWGSensorsOperationRequest() {
    init();
}

SWGSensorsOperationRequest::~SWGSensorsOperationRequest() {
    this->cleanup();
}

void
SWGSensorsOperationRequest::init() {
    operation = new QString("");
    m_operation_isSet = false;
    sensor = new SWGSensorsParameter();
    m_sensor_isSet = false;
}

void
SWGSensorsOperationRequest::cleanup() {
    if(operation != nullptr) { 
        delete operation;
    }
    if(sensor != nullptr) { 
        delete sensor;
    }
}

SWGSensorsOperationRequest*
SWGSensorsOperationRequest::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGSensorsOperationRequest::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&operation, pJson["operation"], "QString", "QString");
    
    ::Swagger::setValue(&sensor, pJson["sensor"], "SWGSensorsParameter", "SWGSensorsParameter");
    
}

QString
SWGSensorsOperationRequest::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGSensorsOperationRequest::asJsonObject() {
    QJsonObject obj;
    if(operation != nullptr && *operation != QString("")){
        toJsonValue(QString("operation"), operation, obj, QString("QString"));
    }
    if((sensor != nullptr) && (sensor->isSet())){
        toJsonValue(QString("sensor"), sensor, obj, QString("SWGSensorsParameter"));
    }

    return obj;
}

QString*
SWGSensorsOperationRequest::getOperation() {
    return operation;
}
void
SWGSensorsOperationRequest::setOperation(QString* operation) {
    this->operation = operation;
    this->m_operation_isSet = true;
}

SWGSensorsParameter*
SWGSensorsOperationRequest::getSensor() {
    return sensor;
}
void
SWGSensorsOperationRequest::setSensor(SWGSensorsParameter* sensor) {
    this->sensor = sensor;
    this->m_sensor_isSet = true;
}


bool
SWGSensorsOperationRequest::isSet(){
    bool isObjectUpdated = false;
    do{
        if(operation != nullptr && *operation != QString("")){ isObjectUpdated = true; break;}
        if(sensor != nullptr && sensor->isSet()){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

