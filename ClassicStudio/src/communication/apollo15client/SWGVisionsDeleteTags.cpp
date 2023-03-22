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


#include "SWGVisionsDeleteTags.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGVisionsDeleteTags::SWGVisionsDeleteTags(QString json) {
    init();
    this->fromJson(json);
}

SWGVisionsDeleteTags::SWGVisionsDeleteTags() {
    init();
}

SWGVisionsDeleteTags::~SWGVisionsDeleteTags() {
    this->cleanup();
}

void
SWGVisionsDeleteTags::init() {
    tags = new QString("");
    m_tags_isSet = false;
    mode = new QString("");
    m_mode_isSet = false;
}

void
SWGVisionsDeleteTags::cleanup() {
    if(tags != nullptr) { 
        delete tags;
    }
    if(mode != nullptr) { 
        delete mode;
    }
}

SWGVisionsDeleteTags*
SWGVisionsDeleteTags::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGVisionsDeleteTags::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&tags, pJson["tags"], "QString", "QString");
    
    ::Swagger::setValue(&mode, pJson["mode"], "QString", "QString");
    
}

QString
SWGVisionsDeleteTags::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGVisionsDeleteTags::asJsonObject() {
    QJsonObject obj;
    if(tags != nullptr && *tags != QString("")){
        toJsonValue(QString("tags"), tags, obj, QString("QString"));
    }
    if(mode != nullptr && *mode != QString("")){
        toJsonValue(QString("mode"), mode, obj, QString("QString"));
    }

    return obj;
}

QString*
SWGVisionsDeleteTags::getTags() {
    return tags;
}
void
SWGVisionsDeleteTags::setTags(QString* tags) {
    this->tags = tags;
    this->m_tags_isSet = true;
}

QString*
SWGVisionsDeleteTags::getMode() {
    return mode;
}
void
SWGVisionsDeleteTags::setMode(QString* mode) {
    this->mode = mode;
    this->m_mode_isSet = true;
}


bool
SWGVisionsDeleteTags::isSet(){
    bool isObjectUpdated = false;
    do{
        if(tags != nullptr && *tags != QString("")){ isObjectUpdated = true; break;}
        if(mode != nullptr && *mode != QString("")){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

