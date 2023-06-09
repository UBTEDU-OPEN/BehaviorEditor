﻿/**
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


#include "SWGServosAngles.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace Swagger {

SWGServosAngles::SWGServosAngles(QString json) {
    init();
    this->fromJson(json);
}

SWGServosAngles::SWGServosAngles() {
    init();
}

SWGServosAngles::~SWGServosAngles() {
    this->cleanup();
}

void
SWGServosAngles::init() {
    head_pitch = 0;
    m_head_pitch_isSet = false;
    head_yaw = 0;
    m_head_yaw_isSet = false;
    r_shoulder_pitch = 0;
    m_r_shoulder_pitch_isSet = false;
    r_shoulder_roll = 0;
    m_r_shoulder_roll_isSet = false;
    r_elbow_roll = 0;
    m_r_elbow_roll_isSet = false;
    r_hand = 0;
    m_r_hand_isSet = false;
    l_shoulder_pitch = 0;
    m_l_shoulder_pitch_isSet = false;
    l_shoulder_roll = 0;
    m_l_shoulder_roll_isSet = false;
    l_elbow_roll = 0;
    m_l_elbow_roll_isSet = false;
    l_hand = 0;
    m_l_hand_isSet = false;
    r_hip_yaw = 0;
    m_r_hip_yaw_isSet = false;
    r_hip_roll = 0;
    m_r_hip_roll_isSet = false;
    r_hip_pitch = 0;
    m_r_hip_pitch_isSet = false;
    r_knee_pitch = 0;
    m_r_knee_pitch_isSet = false;
    r_ankle_pitch = 0;
    m_r_ankle_pitch_isSet = false;
    r_ankle_roll = 0;
    m_r_ankle_roll_isSet = false;
    l_hip_yaw = 0;
    m_l_hip_yaw_isSet = false;
    l_hip_roll = 0;
    m_l_hip_roll_isSet = false;
    l_hip_pitch = 0;
    m_l_hip_pitch_isSet = false;
    l_knee_pitch = 0;
    m_l_knee_pitch_isSet = false;
    l_ankle_pitch = 0;
    m_l_ankle_pitch_isSet = false;
    l_ankle_roll = 0;
    m_l_ankle_roll_isSet = false;
}

void
SWGServosAngles::cleanup() {






















}

SWGServosAngles*
SWGServosAngles::fromJson(QString json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

//added 2020-12-17
void SWGServosAngles::setIsSetFlag(const QJsonObject& obj)
{
    if(obj.contains("HeadPitch")) { m_head_pitch_isSet = true; }
    if(obj.contains("HeadYaw")) { m_head_yaw_isSet = true; }
    if(obj.contains("RShoulderPitch"))    { m_r_shoulder_pitch_isSet = true; }
    if(obj.contains("RShoulderRoll"))  { m_r_shoulder_roll_isSet = true; }
    if(obj.contains("RElbowRoll"))  { m_r_elbow_roll_isSet = true; }
    if(obj.contains("RHand"))     { m_r_hand_isSet = true; }
    if(obj.contains("LShoulderPitch"))        { m_l_shoulder_pitch_isSet = true; }
    if(obj.contains("LShoulderRoll"))        { m_l_shoulder_roll_isSet = true; }
    if(obj.contains("LElbowRoll"))     { m_l_elbow_roll_isSet = true; }
    if(obj.contains("LHand"))      { m_l_hand_isSet = true; }
    if(obj.contains("RHipYaw"))      { m_r_hip_yaw_isSet = true; }
    if(obj.contains("RHipRoll"))         { m_r_hip_roll_isSet = true; }
    if(obj.contains("RHipPitch"))         { m_r_hip_pitch_isSet = true; }
    if(obj.contains("RKneePitch"))      { m_r_knee_pitch_isSet = true; }
    if(obj.contains("RAnklePitch"))       { m_r_ankle_pitch_isSet = true; }
    if(obj.contains("RAnkleRoll"))       { m_r_ankle_roll_isSet = true; }
    if(obj.contains("LHipYaw"))            { m_l_hip_yaw_isSet = true; }
    if(obj.contains("LHipRoll"))         { m_l_hip_roll_isSet = true; }
    if(obj.contains("LHipPitch"))         { m_l_hip_pitch_isSet = true; }
    if(obj.contains("LKneePitch"))      { m_l_knee_pitch_isSet = true; }
    if(obj.contains("LAnklePitch"))       { m_l_ankle_pitch_isSet = true; }
    if(obj.contains("LAnkleRoll"))       { m_l_ankle_roll_isSet = true; }
}

void
SWGServosAngles::fromJsonObject(QJsonObject pJson) {
    QJsonObject tempObj = pJson; //added 2020-12-17

    ::Swagger::setValue(&head_pitch, pJson["HeadPitch"], "qint32", "");
    
    ::Swagger::setValue(&head_yaw, pJson["HeadYaw"], "qint32", "");
    
    ::Swagger::setValue(&r_shoulder_pitch, pJson["RShoulderPitch"], "qint32", "");
    
    ::Swagger::setValue(&r_shoulder_roll, pJson["RShoulderRoll"], "qint32", "");
    
    ::Swagger::setValue(&r_elbow_roll, pJson["RElbowRoll"], "qint32", "");
    
    ::Swagger::setValue(&r_hand, pJson["RHand"], "qint32", "");
    
    ::Swagger::setValue(&l_shoulder_pitch, pJson["LShoulderPitch"], "qint32", "");
    
    ::Swagger::setValue(&l_shoulder_roll, pJson["LShoulderRoll"], "qint32", "");
    
    ::Swagger::setValue(&l_elbow_roll, pJson["LElbowRoll"], "qint32", "");
    
    ::Swagger::setValue(&l_hand, pJson["LHand"], "qint32", "");
    
    ::Swagger::setValue(&r_hip_yaw, pJson["RHipYaw"], "qint32", "");
    
    ::Swagger::setValue(&r_hip_roll, pJson["RHipRoll"], "qint32", "");
    
    ::Swagger::setValue(&r_hip_pitch, pJson["RHipPitch"], "qint32", "");
    
    ::Swagger::setValue(&r_knee_pitch, pJson["RKneePitch"], "qint32", "");
    
    ::Swagger::setValue(&r_ankle_pitch, pJson["RAnklePitch"], "qint32", "");
    
    ::Swagger::setValue(&r_ankle_roll, pJson["RAnkleRoll"], "qint32", "");
    
    ::Swagger::setValue(&l_hip_yaw, pJson["LHipYaw"], "qint32", "");
    
    ::Swagger::setValue(&l_hip_roll, pJson["LHipRoll"], "qint32", "");
    
    ::Swagger::setValue(&l_hip_pitch, pJson["LHipPitch"], "qint32", "");
    
    ::Swagger::setValue(&l_knee_pitch, pJson["LKneePitch"], "qint32", "");
    
    ::Swagger::setValue(&l_ankle_pitch, pJson["LAnklePitch"], "qint32", "");
    
    ::Swagger::setValue(&l_ankle_roll, pJson["LAnkleRoll"], "qint32", "");
    
    setIsSetFlag(tempObj); //added 2020-12-17
}

QString
SWGServosAngles::asJson ()
{
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject
SWGServosAngles::asJsonObject() {
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

qint32
SWGServosAngles::getHeadPitch() {
    return head_pitch;
}
void
SWGServosAngles::setHeadPitch(qint32 head_pitch) {
    this->head_pitch = head_pitch;
    this->m_head_pitch_isSet = true;
}

qint32
SWGServosAngles::getHeadYaw() {
    return head_yaw;
}
void
SWGServosAngles::setHeadYaw(qint32 head_yaw) {
    this->head_yaw = head_yaw;
    this->m_head_yaw_isSet = true;
}

qint32
SWGServosAngles::getRShoulderPitch() {
    return r_shoulder_pitch;
}
void
SWGServosAngles::setRShoulderPitch(qint32 r_shoulder_pitch) {
    this->r_shoulder_pitch = r_shoulder_pitch;
    this->m_r_shoulder_pitch_isSet = true;
}

qint32
SWGServosAngles::getRShoulderRoll() {
    return r_shoulder_roll;
}
void
SWGServosAngles::setRShoulderRoll(qint32 r_shoulder_roll) {
    this->r_shoulder_roll = r_shoulder_roll;
    this->m_r_shoulder_roll_isSet = true;
}

qint32
SWGServosAngles::getRElbowRoll() {
    return r_elbow_roll;
}
void
SWGServosAngles::setRElbowRoll(qint32 r_elbow_roll) {
    this->r_elbow_roll = r_elbow_roll;
    this->m_r_elbow_roll_isSet = true;
}

qint32
SWGServosAngles::getRHand() {
    return r_hand;
}
void
SWGServosAngles::setRHand(qint32 r_hand) {
    this->r_hand = r_hand;
    this->m_r_hand_isSet = true;
}

qint32
SWGServosAngles::getLShoulderPitch() {
    return l_shoulder_pitch;
}
void
SWGServosAngles::setLShoulderPitch(qint32 l_shoulder_pitch) {
    this->l_shoulder_pitch = l_shoulder_pitch;
    this->m_l_shoulder_pitch_isSet = true;
}

qint32
SWGServosAngles::getLShoulderRoll() {
    return l_shoulder_roll;
}
void
SWGServosAngles::setLShoulderRoll(qint32 l_shoulder_roll) {
    this->l_shoulder_roll = l_shoulder_roll;
    this->m_l_shoulder_roll_isSet = true;
}

qint32
SWGServosAngles::getLElbowRoll() {
    return l_elbow_roll;
}
void
SWGServosAngles::setLElbowRoll(qint32 l_elbow_roll) {
    this->l_elbow_roll = l_elbow_roll;
    this->m_l_elbow_roll_isSet = true;
}

qint32
SWGServosAngles::getLHand() {
    return l_hand;
}
void
SWGServosAngles::setLHand(qint32 l_hand) {
    this->l_hand = l_hand;
    this->m_l_hand_isSet = true;
}

qint32
SWGServosAngles::getRHipYaw() {
    return r_hip_yaw;
}
void
SWGServosAngles::setRHipYaw(qint32 r_hip_yaw) {
    this->r_hip_yaw = r_hip_yaw;
    this->m_r_hip_yaw_isSet = true;
}

qint32
SWGServosAngles::getRHipRoll() {
    return r_hip_roll;
}
void
SWGServosAngles::setRHipRoll(qint32 r_hip_roll) {
    this->r_hip_roll = r_hip_roll;
    this->m_r_hip_roll_isSet = true;
}

qint32
SWGServosAngles::getRHipPitch() {
    return r_hip_pitch;
}
void
SWGServosAngles::setRHipPitch(qint32 r_hip_pitch) {
    this->r_hip_pitch = r_hip_pitch;
    this->m_r_hip_pitch_isSet = true;
}

qint32
SWGServosAngles::getRKneePitch() {
    return r_knee_pitch;
}
void
SWGServosAngles::setRKneePitch(qint32 r_knee_pitch) {
    this->r_knee_pitch = r_knee_pitch;
    this->m_r_knee_pitch_isSet = true;
}

qint32
SWGServosAngles::getRAnklePitch() {
    return r_ankle_pitch;
}
void
SWGServosAngles::setRAnklePitch(qint32 r_ankle_pitch) {
    this->r_ankle_pitch = r_ankle_pitch;
    this->m_r_ankle_pitch_isSet = true;
}

qint32
SWGServosAngles::getRAnkleRoll() {
    return r_ankle_roll;
}
void
SWGServosAngles::setRAnkleRoll(qint32 r_ankle_roll) {
    this->r_ankle_roll = r_ankle_roll;
    this->m_r_ankle_roll_isSet = true;
}

qint32
SWGServosAngles::getLHipYaw() {
    return l_hip_yaw;
}
void
SWGServosAngles::setLHipYaw(qint32 l_hip_yaw) {
    this->l_hip_yaw = l_hip_yaw;
    this->m_l_hip_yaw_isSet = true;
}

qint32
SWGServosAngles::getLHipRoll() {
    return l_hip_roll;
}
void
SWGServosAngles::setLHipRoll(qint32 l_hip_roll) {
    this->l_hip_roll = l_hip_roll;
    this->m_l_hip_roll_isSet = true;
}

qint32
SWGServosAngles::getLHipPitch() {
    return l_hip_pitch;
}
void
SWGServosAngles::setLHipPitch(qint32 l_hip_pitch) {
    this->l_hip_pitch = l_hip_pitch;
    this->m_l_hip_pitch_isSet = true;
}

qint32
SWGServosAngles::getLKneePitch() {
    return l_knee_pitch;
}
void
SWGServosAngles::setLKneePitch(qint32 l_knee_pitch) {
    this->l_knee_pitch = l_knee_pitch;
    this->m_l_knee_pitch_isSet = true;
}

qint32
SWGServosAngles::getLAnklePitch() {
    return l_ankle_pitch;
}
void
SWGServosAngles::setLAnklePitch(qint32 l_ankle_pitch) {
    this->l_ankle_pitch = l_ankle_pitch;
    this->m_l_ankle_pitch_isSet = true;
}

qint32
SWGServosAngles::getLAnkleRoll() {
    return l_ankle_roll;
}
void
SWGServosAngles::setLAnkleRoll(qint32 l_ankle_roll) {
    this->l_ankle_roll = l_ankle_roll;
    this->m_l_ankle_roll_isSet = true;
}


bool
SWGServosAngles::isSet(){
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

