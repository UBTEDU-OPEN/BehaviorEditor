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


#include "SWGVoiceTTSStr.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGVoiceTTSStr::SWGVoiceTTSStr(QString json) {
    init();
    this->fromJson(json);
}

SWGVoiceTTSStr::SWGVoiceTTSStr() {
    init();
}

SWGVoiceTTSStr::~SWGVoiceTTSStr() {
    this->cleanup();
}

void
SWGVoiceTTSStr::init() {
    tts = new QString("");
    m_tts_isSet = false;
    interrupt = false;
    m_interrupt_isSet = false;
    timestamp = 0L;
    m_timestamp_isSet = false;
}

void
SWGVoiceTTSStr::cleanup() {
    if(tts != nullptr) { 
        delete tts;
    }


}

SWGVoiceTTSStr*
SWGVoiceTTSStr::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGVoiceTTSStr::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&tts, pJson["tts"], "QString", "QString");
    
    ::Swagger::setValue(&interrupt, pJson["interrupt"], "bool", "");
    
    ::Swagger::setValue(&timestamp, pJson["timestamp"], "qint64", "");
    
}

QString
SWGVoiceTTSStr::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGVoiceTTSStr::asJsonObject() {
    QJsonObject obj;
    if(tts != nullptr && *tts != QString("")){
        toJsonValue(QString("tts"), tts, obj, QString("QString"));
    }
    if(m_interrupt_isSet){
        obj.insert("interrupt", QJsonValue(interrupt));
    }
    if(m_timestamp_isSet){
        obj.insert("timestamp", QJsonValue(timestamp));
    }

    return obj;
}

QString*
SWGVoiceTTSStr::getTts() {
    return tts;
}
void
SWGVoiceTTSStr::setTts(QString* tts) {
    this->tts = tts;
    this->m_tts_isSet = true;
}

bool
SWGVoiceTTSStr::isInterrupt() {
    return interrupt;
}
void
SWGVoiceTTSStr::setInterrupt(bool interrupt) {
    this->interrupt = interrupt;
    this->m_interrupt_isSet = true;
}

qint64
SWGVoiceTTSStr::getTimestamp() {
    return timestamp;
}
void
SWGVoiceTTSStr::setTimestamp(qint64 timestamp) {
    this->timestamp = timestamp;
    this->m_timestamp_isSet = true;
}


bool
SWGVoiceTTSStr::isSet(){
    bool isObjectUpdated = false;
    do{
        if(tts != nullptr && *tts != QString("")){ isObjectUpdated = true; break;}
        if(m_interrupt_isSet){ isObjectUpdated = true; break;}
        if(m_timestamp_isSet){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

