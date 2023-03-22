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


#include "SWGServosResult.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGServosResult::SWGServosResult(QString json) {
    init();
    this->fromJson(json);
}

SWGServosResult::SWGServosResult() {
    init();
}

SWGServosResult::~SWGServosResult() {
    this->cleanup();
}

void
SWGServosResult::init() {
    head_pitch = false;
    m_head_pitch_isSet = false;
    head_yaw = false;
    m_head_yaw_isSet = false;
    r_shoulder_pitch = false;
    m_r_shoulder_pitch_isSet = false;
    r_shoulder_roll = false;
    m_r_shoulder_roll_isSet = false;
    r_elbow_roll = false;
    m_r_elbow_roll_isSet = false;
    r_hand = false;
    m_r_hand_isSet = false;
    l_shoulder_pitch = false;
    m_l_shoulder_pitch_isSet = false;
    l_shoulder_roll = false;
    m_l_shoulder_roll_isSet = false;
    l_elbow_roll = false;
    m_l_elbow_roll_isSet = false;
    l_hand = false;
    m_l_hand_isSet = false;
    r_hip_yaw = false;
    m_r_hip_yaw_isSet = false;
    r_hip_roll = false;
    m_r_hip_roll_isSet = false;
    r_hip_pitch = false;
    m_r_hip_pitch_isSet = false;
    r_knee_pitch = false;
    m_r_knee_pitch_isSet = false;
    r_ankle_pitch = false;
    m_r_ankle_pitch_isSet = false;
    r_ankle_roll = false;
    m_r_ankle_roll_isSet = false;
    l_hip_yaw = false;
    m_l_hip_yaw_isSet = false;
    l_hip_roll = false;
    m_l_hip_roll_isSet = false;
    l_hip_pitch = false;
    m_l_hip_pitch_isSet = false;
    l_knee_pitch = false;
    m_l_knee_pitch_isSet = false;
    l_ankle_pitch = false;
    m_l_ankle_pitch_isSet = false;
    l_ankle_roll = false;
    m_l_ankle_roll_isSet = false;
}

void
SWGServosResult::cleanup() {






















}

SWGServosResult*
SWGServosResult::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGServosResult::fromJsonObject(QJsonObject pJson) {
    ::Swagger::setValue(&head_pitch, pJson["HeadPitch"], "bool", "");
    
    ::Swagger::setValue(&head_yaw, pJson["HeadYaw"], "bool", "");
    
    ::Swagger::setValue(&r_shoulder_pitch, pJson["RShoulderPitch"], "bool", "");
    
    ::Swagger::setValue(&r_shoulder_roll, pJson["RShoulderRoll"], "bool", "");
    
    ::Swagger::setValue(&r_elbow_roll, pJson["RElbowRoll"], "bool", "");
    
    ::Swagger::setValue(&r_hand, pJson["RHand"], "bool", "");
    
    ::Swagger::setValue(&l_shoulder_pitch, pJson["LShoulderPitch"], "bool", "");
    
    ::Swagger::setValue(&l_shoulder_roll, pJson["LShoulderRoll"], "bool", "");
    
    ::Swagger::setValue(&l_elbow_roll, pJson["LElbowRoll"], "bool", "");
    
    ::Swagger::setValue(&l_hand, pJson["LHand"], "bool", "");
    
    ::Swagger::setValue(&r_hip_yaw, pJson["RHipYaw"], "bool", "");
    
    ::Swagger::setValue(&r_hip_roll, pJson["RHipRoll"], "bool", "");
    
    ::Swagger::setValue(&r_hip_pitch, pJson["RHipPitch"], "bool", "");
    
    ::Swagger::setValue(&r_knee_pitch, pJson["RKneePitch"], "bool", "");
    
    ::Swagger::setValue(&r_ankle_pitch, pJson["RAnklePitch"], "bool", "");
    
    ::Swagger::setValue(&r_ankle_roll, pJson["RAnkleRoll"], "bool", "");
    
    ::Swagger::setValue(&l_hip_yaw, pJson["LHipYaw"], "bool", "");
    
    ::Swagger::setValue(&l_hip_roll, pJson["LHipRoll"], "bool", "");
    
    ::Swagger::setValue(&l_hip_pitch, pJson["LHipPitch"], "bool", "");
    
    ::Swagger::setValue(&l_knee_pitch, pJson["LKneePitch"], "bool", "");
    
    ::Swagger::setValue(&l_ankle_pitch, pJson["LAnklePitch"], "bool", "");
    
    ::Swagger::setValue(&l_ankle_roll, pJson["LAnkleRoll"], "bool", "");
    
}

QString
SWGServosResult::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGServosResult::asJsonObject() {
    QJsonObject obj;
    if(m_head_pitch_isSet){
        obj.insert("HeadPitch", QJsonValue(head_pitch));
    }
    if(m_head_yaw_isSet){
        obj.insert("HeadYaw", QJsonValue(head_yaw));
    }
    if(m_r_shoulder_pitch_isSet){
        obj.insert("RShoulderPitch", QJsonValue(r_shoulder_pitch));
    }
    if(m_r_shoulder_roll_isSet){
        obj.insert("RShoulderRoll", QJsonValue(r_shoulder_roll));
    }
    if(m_r_elbow_roll_isSet){
        obj.insert("RElbowRoll", QJsonValue(r_elbow_roll));
    }
    if(m_r_hand_isSet){
        obj.insert("RHand", QJsonValue(r_hand));
    }
    if(m_l_shoulder_pitch_isSet){
        obj.insert("LShoulderPitch", QJsonValue(l_shoulder_pitch));
    }
    if(m_l_shoulder_roll_isSet){
        obj.insert("LShoulderRoll", QJsonValue(l_shoulder_roll));
    }
    if(m_l_elbow_roll_isSet){
        obj.insert("LElbowRoll", QJsonValue(l_elbow_roll));
    }
    if(m_l_hand_isSet){
        obj.insert("LHand", QJsonValue(l_hand));
    }
    if(m_r_hip_yaw_isSet){
        obj.insert("RHipYaw", QJsonValue(r_hip_yaw));
    }
    if(m_r_hip_roll_isSet){
        obj.insert("RHipRoll", QJsonValue(r_hip_roll));
    }
    if(m_r_hip_pitch_isSet){
        obj.insert("RHipPitch", QJsonValue(r_hip_pitch));
    }
    if(m_r_knee_pitch_isSet){
        obj.insert("RKneePitch", QJsonValue(r_knee_pitch));
    }
    if(m_r_ankle_pitch_isSet){
        obj.insert("RAnklePitch", QJsonValue(r_ankle_pitch));
    }
    if(m_r_ankle_roll_isSet){
        obj.insert("RAnkleRoll", QJsonValue(r_ankle_roll));
    }
    if(m_l_hip_yaw_isSet){
        obj.insert("LHipYaw", QJsonValue(l_hip_yaw));
    }
    if(m_l_hip_roll_isSet){
        obj.insert("LHipRoll", QJsonValue(l_hip_roll));
    }
    if(m_l_hip_pitch_isSet){
        obj.insert("LHipPitch", QJsonValue(l_hip_pitch));
    }
    if(m_l_knee_pitch_isSet){
        obj.insert("LKneePitch", QJsonValue(l_knee_pitch));
    }
    if(m_l_ankle_pitch_isSet){
        obj.insert("LAnklePitch", QJsonValue(l_ankle_pitch));
    }
    if(m_l_ankle_roll_isSet){
        obj.insert("LAnkleRoll", QJsonValue(l_ankle_roll));
    }

    return obj;
}

bool
SWGServosResult::isHeadPitch() {
    return head_pitch;
}
void
SWGServosResult::setHeadPitch(bool head_pitch) {
    this->head_pitch = head_pitch;
    this->m_head_pitch_isSet = true;
}

bool
SWGServosResult::isHeadYaw() {
    return head_yaw;
}
void
SWGServosResult::setHeadYaw(bool head_yaw) {
    this->head_yaw = head_yaw;
    this->m_head_yaw_isSet = true;
}

bool
SWGServosResult::isRShoulderPitch() {
    return r_shoulder_pitch;
}
void
SWGServosResult::setRShoulderPitch(bool r_shoulder_pitch) {
    this->r_shoulder_pitch = r_shoulder_pitch;
    this->m_r_shoulder_pitch_isSet = true;
}

bool
SWGServosResult::isRShoulderRoll() {
    return r_shoulder_roll;
}
void
SWGServosResult::setRShoulderRoll(bool r_shoulder_roll) {
    this->r_shoulder_roll = r_shoulder_roll;
    this->m_r_shoulder_roll_isSet = true;
}

bool
SWGServosResult::isRElbowRoll() {
    return r_elbow_roll;
}
void
SWGServosResult::setRElbowRoll(bool r_elbow_roll) {
    this->r_elbow_roll = r_elbow_roll;
    this->m_r_elbow_roll_isSet = true;
}

bool
SWGServosResult::isRHand() {
    return r_hand;
}
void
SWGServosResult::setRHand(bool r_hand) {
    this->r_hand = r_hand;
    this->m_r_hand_isSet = true;
}

bool
SWGServosResult::isLShoulderPitch() {
    return l_shoulder_pitch;
}
void
SWGServosResult::setLShoulderPitch(bool l_shoulder_pitch) {
    this->l_shoulder_pitch = l_shoulder_pitch;
    this->m_l_shoulder_pitch_isSet = true;
}

bool
SWGServosResult::isLShoulderRoll() {
    return l_shoulder_roll;
}
void
SWGServosResult::setLShoulderRoll(bool l_shoulder_roll) {
    this->l_shoulder_roll = l_shoulder_roll;
    this->m_l_shoulder_roll_isSet = true;
}

bool
SWGServosResult::isLElbowRoll() {
    return l_elbow_roll;
}
void
SWGServosResult::setLElbowRoll(bool l_elbow_roll) {
    this->l_elbow_roll = l_elbow_roll;
    this->m_l_elbow_roll_isSet = true;
}

bool
SWGServosResult::isLHand() {
    return l_hand;
}
void
SWGServosResult::setLHand(bool l_hand) {
    this->l_hand = l_hand;
    this->m_l_hand_isSet = true;
}

bool
SWGServosResult::isRHipYaw() {
    return r_hip_yaw;
}
void
SWGServosResult::setRHipYaw(bool r_hip_yaw) {
    this->r_hip_yaw = r_hip_yaw;
    this->m_r_hip_yaw_isSet = true;
}

bool
SWGServosResult::isRHipRoll() {
    return r_hip_roll;
}
void
SWGServosResult::setRHipRoll(bool r_hip_roll) {
    this->r_hip_roll = r_hip_roll;
    this->m_r_hip_roll_isSet = true;
}

bool
SWGServosResult::isRHipPitch() {
    return r_hip_pitch;
}
void
SWGServosResult::setRHipPitch(bool r_hip_pitch) {
    this->r_hip_pitch = r_hip_pitch;
    this->m_r_hip_pitch_isSet = true;
}

bool
SWGServosResult::isRKneePitch() {
    return r_knee_pitch;
}
void
SWGServosResult::setRKneePitch(bool r_knee_pitch) {
    this->r_knee_pitch = r_knee_pitch;
    this->m_r_knee_pitch_isSet = true;
}

bool
SWGServosResult::isRAnklePitch() {
    return r_ankle_pitch;
}
void
SWGServosResult::setRAnklePitch(bool r_ankle_pitch) {
    this->r_ankle_pitch = r_ankle_pitch;
    this->m_r_ankle_pitch_isSet = true;
}

bool
SWGServosResult::isRAnkleRoll() {
    return r_ankle_roll;
}
void
SWGServosResult::setRAnkleRoll(bool r_ankle_roll) {
    this->r_ankle_roll = r_ankle_roll;
    this->m_r_ankle_roll_isSet = true;
}

bool
SWGServosResult::isLHipYaw() {
    return l_hip_yaw;
}
void
SWGServosResult::setLHipYaw(bool l_hip_yaw) {
    this->l_hip_yaw = l_hip_yaw;
    this->m_l_hip_yaw_isSet = true;
}

bool
SWGServosResult::isLHipRoll() {
    return l_hip_roll;
}
void
SWGServosResult::setLHipRoll(bool l_hip_roll) {
    this->l_hip_roll = l_hip_roll;
    this->m_l_hip_roll_isSet = true;
}

bool
SWGServosResult::isLHipPitch() {
    return l_hip_pitch;
}
void
SWGServosResult::setLHipPitch(bool l_hip_pitch) {
    this->l_hip_pitch = l_hip_pitch;
    this->m_l_hip_pitch_isSet = true;
}

bool
SWGServosResult::isLKneePitch() {
    return l_knee_pitch;
}
void
SWGServosResult::setLKneePitch(bool l_knee_pitch) {
    this->l_knee_pitch = l_knee_pitch;
    this->m_l_knee_pitch_isSet = true;
}

bool
SWGServosResult::isLAnklePitch() {
    return l_ankle_pitch;
}
void
SWGServosResult::setLAnklePitch(bool l_ankle_pitch) {
    this->l_ankle_pitch = l_ankle_pitch;
    this->m_l_ankle_pitch_isSet = true;
}

bool
SWGServosResult::isLAnkleRoll() {
    return l_ankle_roll;
}
void
SWGServosResult::setLAnkleRoll(bool l_ankle_roll) {
    this->l_ankle_roll = l_ankle_roll;
    this->m_l_ankle_roll_isSet = true;
}


bool
SWGServosResult::isSet(){
    bool isObjectUpdated = false;
    do{
        if(m_head_pitch_isSet){ isObjectUpdated = true; break;}
        if(m_head_yaw_isSet){ isObjectUpdated = true; break;}
        if(m_r_shoulder_pitch_isSet){ isObjectUpdated = true; break;}
        if(m_r_shoulder_roll_isSet){ isObjectUpdated = true; break;}
        if(m_r_elbow_roll_isSet){ isObjectUpdated = true; break;}
        if(m_r_hand_isSet){ isObjectUpdated = true; break;}
        if(m_l_shoulder_pitch_isSet){ isObjectUpdated = true; break;}
        if(m_l_shoulder_roll_isSet){ isObjectUpdated = true; break;}
        if(m_l_elbow_roll_isSet){ isObjectUpdated = true; break;}
        if(m_l_hand_isSet){ isObjectUpdated = true; break;}
        if(m_r_hip_yaw_isSet){ isObjectUpdated = true; break;}
        if(m_r_hip_roll_isSet){ isObjectUpdated = true; break;}
        if(m_r_hip_pitch_isSet){ isObjectUpdated = true; break;}
        if(m_r_knee_pitch_isSet){ isObjectUpdated = true; break;}
        if(m_r_ankle_pitch_isSet){ isObjectUpdated = true; break;}
        if(m_r_ankle_roll_isSet){ isObjectUpdated = true; break;}
        if(m_l_hip_yaw_isSet){ isObjectUpdated = true; break;}
        if(m_l_hip_roll_isSet){ isObjectUpdated = true; break;}
        if(m_l_hip_pitch_isSet){ isObjectUpdated = true; break;}
        if(m_l_knee_pitch_isSet){ isObjectUpdated = true; break;}
        if(m_l_ankle_pitch_isSet){ isObjectUpdated = true; break;}
        if(m_l_ankle_roll_isSet){ isObjectUpdated = true; break;}
    }while(false);
    return isObjectUpdated;
}
}
