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


#include "SWGMediaMusicStatus.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGMediaMusicStatus::SWGMediaMusicStatus(QString json) {
    init();
    this->fromJson(json);
}

SWGMediaMusicStatus::SWGMediaMusicStatus() {
    init();
}

SWGMediaMusicStatus::~SWGMediaMusicStatus() {
    this->cleanup();
}

void
SWGMediaMusicStatus::init() {
    status = new QString("");
    m_status_isSet = false;
    name = new QString("");
    m_name_isSet = false;
}

void
SWGMediaMusicStatus::cleanup() {
    if(status != nullptr) { 
        delete status;
    }
    if(name != nullptr) { 
        delete name;
    }
}

SWGMediaMusicStatus*
SWGMediaMusicStatus::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGMediaMusicStatus::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&status, pJson["status"], "QString", "QString");
    
    ::Swagger::setValue(&name, pJson["name"], "QString", "QString");
    
}

QString
SWGMediaMusicStatus::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGMediaMusicStatus::asJsonObject() {
    QJsonObject obj;
    if(status != nullptr && *status != QString("")){
        toJsonValue(QString("status"), status, obj, QString("QString"));
    }
    if(name != nullptr && *name != QString("")){
        toJsonValue(QString("name"), name, obj, QString("QString"));
    }

    return obj;
}

QString*
SWGMediaMusicStatus::getStatus() {
    return status;
}
void
SWGMediaMusicStatus::setStatus(QString* status) {
    this->status = status;
    this->m_status_isSet = true;
}

QString*
SWGMediaMusicStatus::getName() {
    return name;
}
void
SWGMediaMusicStatus::setName(QString* name) {
    this->name = name;
    this->m_name_isSet = true;
}


bool
SWGMediaMusicStatus::isSet(){
    bool isObjectUpdated = false;
    do{
        if(status != nullptr && *status != QString("")){ isObjectUpdated = true; break;}
        if(name != nullptr && *name != QString("")){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

