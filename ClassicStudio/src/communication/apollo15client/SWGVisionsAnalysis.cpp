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


#include "SWGVisionsAnalysis.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGVisionsAnalysis::SWGVisionsAnalysis(QString json) {
    init();
    this->fromJson(json);
}

SWGVisionsAnalysis::SWGVisionsAnalysis() {
    init();
}

SWGVisionsAnalysis::~SWGVisionsAnalysis() {
    this->cleanup();
}

void
SWGVisionsAnalysis::init() {
    age = 0;
    m_age_isSet = false;
    group = new QString("");
    m_group_isSet = false;
    gender = new QString("");
    m_gender_isSet = false;
    glass = new QString("");
    m_glass_isSet = false;
    mask = new QString("");
    m_mask_isSet = false;
    expression = new QString("");
    m_expression_isSet = false;
}

void
SWGVisionsAnalysis::cleanup() {

    if(group != nullptr) { 
        delete group;
    }
    if(gender != nullptr) { 
        delete gender;
    }
    if(glass != nullptr) { 
        delete glass;
    }
    if(mask != nullptr) { 
        delete mask;
    }
    if(expression != nullptr) { 
        delete expression;
    }
}

SWGVisionsAnalysis*
SWGVisionsAnalysis::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGVisionsAnalysis::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&age, pJson["age"], "qint32", "");
    
    ::Swagger::setValue(&group, pJson["group"], "QString", "QString");
    
    ::Swagger::setValue(&gender, pJson["gender"], "QString", "QString");
    
    ::Swagger::setValue(&glass, pJson["glass"], "QString", "QString");
    
    ::Swagger::setValue(&mask, pJson["mask"], "QString", "QString");
    
    ::Swagger::setValue(&expression, pJson["expression"], "QString", "QString");
    
}

QString
SWGVisionsAnalysis::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGVisionsAnalysis::asJsonObject() {
    QJsonObject obj;
    if(m_age_isSet){
        obj.insert("age", QJsonValue(age));
    }
    if(group != nullptr && *group != QString("")){
        toJsonValue(QString("group"), group, obj, QString("QString"));
    }
    if(gender != nullptr && *gender != QString("")){
        toJsonValue(QString("gender"), gender, obj, QString("QString"));
    }
    if(glass != nullptr && *glass != QString("")){
        toJsonValue(QString("glass"), glass, obj, QString("QString"));
    }
    if(mask != nullptr && *mask != QString("")){
        toJsonValue(QString("mask"), mask, obj, QString("QString"));
    }
    if(expression != nullptr && *expression != QString("")){
        toJsonValue(QString("expression"), expression, obj, QString("QString"));
    }

    return obj;
}

qint32
SWGVisionsAnalysis::getAge() {
    return age;
}
void
SWGVisionsAnalysis::setAge(qint32 age) {
    this->age = age;
    this->m_age_isSet = true;
}

QString*
SWGVisionsAnalysis::getGroup() {
    return group;
}
void
SWGVisionsAnalysis::setGroup(QString* group) {
    this->group = group;
    this->m_group_isSet = true;
}

QString*
SWGVisionsAnalysis::getGender() {
    return gender;
}
void
SWGVisionsAnalysis::setGender(QString* gender) {
    this->gender = gender;
    this->m_gender_isSet = true;
}

QString*
SWGVisionsAnalysis::getGlass() {
    return glass;
}
void
SWGVisionsAnalysis::setGlass(QString* glass) {
    this->glass = glass;
    this->m_glass_isSet = true;
}

QString*
SWGVisionsAnalysis::getMask() {
    return mask;
}
void
SWGVisionsAnalysis::setMask(QString* mask) {
    this->mask = mask;
    this->m_mask_isSet = true;
}

QString*
SWGVisionsAnalysis::getExpression() {
    return expression;
}
void
SWGVisionsAnalysis::setExpression(QString* expression) {
    this->expression = expression;
    this->m_expression_isSet = true;
}


bool
SWGVisionsAnalysis::isSet(){
    bool isObjectUpdated = false;
    do{
        if(m_age_isSet){ isObjectUpdated = true; break;}
        if(group != nullptr && *group != QString("")){ isObjectUpdated = true; break;}
        if(gender != nullptr && *gender != QString("")){ isObjectUpdated = true; break;}
        if(glass != nullptr && *glass != QString("")){ isObjectUpdated = true; break;}
        if(mask != nullptr && *mask != QString("")){ isObjectUpdated = true; break;}
        if(expression != nullptr && *expression != QString("")){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}
