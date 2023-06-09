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


#include "SWGSensorsList.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGSensorsList::SWGSensorsList(QString json) {
    init();
    this->fromJson(json);
}

SWGSensorsList::SWGSensorsList() {
    init();
}

SWGSensorsList::~SWGSensorsList() {
    this->cleanup();
}

void
SWGSensorsList::init() {
    sensors = new QList<SWGSensorsInfo*>();
    m_sensors_isSet = false;
}

void
SWGSensorsList::cleanup() {
    if(sensors != nullptr) { 
        auto arr = sensors;
        for(auto o: *arr) { 
            delete o;
        }
        delete sensors;
    }
}

SWGSensorsList*
SWGSensorsList::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGSensorsList::fromJsonObject(QJsonObject pJson) {
    
    ::Swagger::setValue(&sensors, pJson["sensors"], "QList", "SWGSensorsInfo");
}

QString
SWGSensorsList::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGSensorsList::asJsonObject() {
    QJsonObject obj;
    if(sensors->size() > 0){
        toJsonArray((QList<void*>*)sensors, obj, "sensors", "SWGSensorsInfo");
    }

    return obj;
}

QList<SWGSensorsInfo*>*
SWGSensorsList::getSensors() {
    return sensors;
}
void
SWGSensorsList::setSensors(QList<SWGSensorsInfo*>* sensors) {
    this->sensors = sensors;
    this->m_sensors_isSet = true;
}


bool
SWGSensorsList::isSet(){
    bool isObjectUpdated = false;
    do{
        if(sensors->size() > 0){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

