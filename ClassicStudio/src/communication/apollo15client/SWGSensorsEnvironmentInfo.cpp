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


#include "SWGSensorsEnvironmentInfo.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGSensorsEnvironmentInfo::SWGSensorsEnvironmentInfo(QString json) {
    init();
    this->fromJson(json);
}

SWGSensorsEnvironmentInfo::SWGSensorsEnvironmentInfo() {
    init();
}

SWGSensorsEnvironmentInfo::~SWGSensorsEnvironmentInfo() {
    this->cleanup();
}

void
SWGSensorsEnvironmentInfo::init() {
    id = 0;
    m_id_isSet = false;
    slot = 0;
    m_slot_isSet = false;
    temperature = 0;
    m_temperature_isSet = false;
    humidity = 0;
    m_humidity_isSet = false;
    pressure = 0;
    m_pressure_isSet = false;
}

void
SWGSensorsEnvironmentInfo::cleanup() {





}

SWGSensorsEnvironmentInfo*
SWGSensorsEnvironmentInfo::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGSensorsEnvironmentInfo::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&id, pJson["id"], "qint32", "");
    
    ::Swagger::setValue(&slot, pJson["slot"], "qint32", "");
    
    ::Swagger::setValue(&temperature, pJson["temperature"], "qint32", "");
    
    ::Swagger::setValue(&humidity, pJson["humidity"], "qint32", "");
    
    ::Swagger::setValue(&pressure, pJson["pressure"], "qint32", "");
    
}

QString
SWGSensorsEnvironmentInfo::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGSensorsEnvironmentInfo::asJsonObject() {
    QJsonObject obj;
    if(m_id_isSet){
        obj.insert("id", QJsonValue(id));
    }
    if(m_slot_isSet){
        obj.insert("slot", QJsonValue(slot));
    }
    if(m_temperature_isSet){
        obj.insert("temperature", QJsonValue(temperature));
    }
    if(m_humidity_isSet){
        obj.insert("humidity", QJsonValue(humidity));
    }
    if(m_pressure_isSet){
        obj.insert("pressure", QJsonValue(pressure));
    }

    return obj;
}

qint32
SWGSensorsEnvironmentInfo::getId() {
    return id;
}
void
SWGSensorsEnvironmentInfo::setId(qint32 id) {
    this->id = id;
    this->m_id_isSet = true;
}

qint32
SWGSensorsEnvironmentInfo::getSlot() {
    return slot;
}
void
SWGSensorsEnvironmentInfo::setSlot(qint32 slot) {
    this->slot = slot;
    this->m_slot_isSet = true;
}

qint32
SWGSensorsEnvironmentInfo::getTemperature() {
    return temperature;
}
void
SWGSensorsEnvironmentInfo::setTemperature(qint32 temperature) {
    this->temperature = temperature;
    this->m_temperature_isSet = true;
}

qint32
SWGSensorsEnvironmentInfo::getHumidity() {
    return humidity;
}
void
SWGSensorsEnvironmentInfo::setHumidity(qint32 humidity) {
    this->humidity = humidity;
    this->m_humidity_isSet = true;
}

qint32
SWGSensorsEnvironmentInfo::getPressure() {
    return pressure;
}
void
SWGSensorsEnvironmentInfo::setPressure(qint32 pressure) {
    this->pressure = pressure;
    this->m_pressure_isSet = true;
}


bool
SWGSensorsEnvironmentInfo::isSet(){
    bool isObjectUpdated = false;
    do{
        if(m_id_isSet){ isObjectUpdated = true; break;}
        if(m_slot_isSet){ isObjectUpdated = true; break;}
        if(m_temperature_isSet){ isObjectUpdated = true; break;}
        if(m_humidity_isSet){ isObjectUpdated = true; break;}
        if(m_pressure_isSet){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

