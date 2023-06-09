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


#include "SWGVisionsPutTags.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGVisionsPutTags::SWGVisionsPutTags(QString json) {
    init();
    this->fromJson(json);
}

SWGVisionsPutTags::SWGVisionsPutTags() {
    init();
}

SWGVisionsPutTags::~SWGVisionsPutTags() {
    this->cleanup();
}

void
SWGVisionsPutTags::init() {
    tags = new QString("");
    m_tags_isSet = false;
    resources = new QList<QString*>();
    m_resources_isSet = false;
}

void
SWGVisionsPutTags::cleanup() {
    if(tags != nullptr) { 
        delete tags;
    }
    if(resources != nullptr) { 
        auto arr = resources;
        for(auto o: *arr) { 
            delete o;
        }
        delete resources;
    }
}

SWGVisionsPutTags*
SWGVisionsPutTags::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGVisionsPutTags::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&tags, pJson["tags"], "QString", "QString");
    
    
    ::Swagger::setValue(&resources, pJson["resources"], "QList", "QString");
}

QString
SWGVisionsPutTags::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGVisionsPutTags::asJsonObject() {
    QJsonObject obj;
    if(tags != nullptr && *tags != QString("")){
        toJsonValue(QString("tags"), tags, obj, QString("QString"));
    }
    if(resources->size() > 0){
        toJsonArray((QList<void*>*)resources, obj, "resources", "QString");
    }

    return obj;
}

QString*
SWGVisionsPutTags::getTags() {
    return tags;
}
void
SWGVisionsPutTags::setTags(QString* tags) {
    this->tags = tags;
    this->m_tags_isSet = true;
}

QList<QString*>*
SWGVisionsPutTags::getResources() {
    return resources;
}
void
SWGVisionsPutTags::setResources(QList<QString*>* resources) {
    this->resources = resources;
    this->m_resources_isSet = true;
}


bool
SWGVisionsPutTags::isSet(){
    bool isObjectUpdated = false;
    do{
        if(tags != nullptr && *tags != QString("")){ isObjectUpdated = true; break;}
        if(resources->size() > 0){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

