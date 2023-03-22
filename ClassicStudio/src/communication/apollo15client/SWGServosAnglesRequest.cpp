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


#include "SWGServosAnglesRequest.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGServosAnglesRequest::SWGServosAnglesRequest(QString json) {
    init();
    this->fromJson(json);
}

SWGServosAnglesRequest::SWGServosAnglesRequest() {
    init();
}

SWGServosAnglesRequest::~SWGServosAnglesRequest() {
    this->cleanup();
}

void
SWGServosAnglesRequest::init() {
    runtime = 0;
    m_runtime_isSet = false;
    angles = new SWGServosAngles();
    m_angles_isSet = false;
}

void
SWGServosAnglesRequest::cleanup() {

    if(angles != nullptr) { 
        delete angles;
    }
}

SWGServosAnglesRequest*
SWGServosAnglesRequest::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

//added 2020-12-17
void SWGServosAnglesRequest::setIsSetFlag(const QJsonObject& obj)
{
    if(obj.contains("runtime")) { m_runtime_isSet = true; }
    if(obj.contains("angles")) { m_angles_isSet = true; }
}

void
SWGServosAnglesRequest::fromJsonObject(QJsonObject pJson) {
    QJsonObject tempObj = pJson; //added 2020-12-17

    ::Swagger::setValue(&runtime, pJson["runtime"], "qint32", "");
    
    ::Swagger::setValue(&angles, pJson["angles"], "SWGServosAngles", "SWGServosAngles");
    
    setIsSetFlag(tempObj); //added 2020-12-17
}

QString
SWGServosAnglesRequest::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGServosAnglesRequest::asJsonObject() {
    QJsonObject obj;
    if(m_runtime_isSet){
        obj.insert("runtime", QJsonValue(runtime));
    }
    if((angles != nullptr) && (angles->isSet())){
        toJsonValue(QString("angles"), angles, obj, QString("SWGServosAngles"));
    }

    return obj;
}

qint32
SWGServosAnglesRequest::getRuntime() {
    return runtime;
}
void
SWGServosAnglesRequest::setRuntime(qint32 runtime) {
    this->runtime = runtime;
    this->m_runtime_isSet = true;
}

SWGServosAngles*
SWGServosAnglesRequest::getAngles() {
    return angles;
}
void
SWGServosAnglesRequest::setAngles(SWGServosAngles* angles) {
    this->angles = angles;
    this->m_angles_isSet = true;
}


bool
SWGServosAnglesRequest::isSet(){
    bool isObjectUpdated = false;
    do{
        if(m_runtime_isSet){ isObjectUpdated = true; break;}
        if(angles != nullptr && angles->isSet()){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}

