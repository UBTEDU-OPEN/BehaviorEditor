﻿/**
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

/*
 * SWGServosAngles.h
 *
 * 
 */

#ifndef SWGServosAngles_H_
#define SWGServosAngles_H_

#include <QJsonObject>



#include "SWGObject.h"

namespace Swagger {

class SWGServosAngles: public SWGObject {
public:
    SWGServosAngles();
    SWGServosAngles(QString json);
    ~SWGServosAngles();
    void init();
    void cleanup();

    QString asJson () override;
    QJsonObject asJsonObject() override;
    void fromJsonObject(QJsonObject json) override;
    SWGServosAngles* fromJson(QString jsonString) override;

    qint32 getRightShoulderRoll();
    void setRightShoulderRoll(qint32 right_shoulder_roll);

    qint32 getRightShoulderFlex();
    void setRightShoulderFlex(qint32 right_shoulder_flex);

    qint32 getRightElbowFlex();
    void setRightElbowFlex(qint32 right_elbow_flex);

    qint32 getLeftShoulderRoll();
    void setLeftShoulderRoll(qint32 left_shoulder_roll);

    qint32 getLeftShoulderFlex();
    void setLeftShoulderFlex(qint32 left_shoulder_flex);

    qint32 getLeftElbowFlex();
    void setLeftElbowFlex(qint32 left_elbow_flex);

    qint32 getRightHipLr();
    void setRightHipLr(qint32 right_hip_lr);

    qint32 getRightHipFb();
    void setRightHipFb(qint32 right_hip_fb);

    qint32 getRightKneeFlex();
    void setRightKneeFlex(qint32 right_knee_flex);

    qint32 getRightAnkleFb();
    void setRightAnkleFb(qint32 right_ankle_fb);

    qint32 getRightAnkleUd();
    void setRightAnkleUd(qint32 right_ankle_ud);

    qint32 getLeftHipLr();
    void setLeftHipLr(qint32 left_hip_lr);

    qint32 getLeftHipFb();
    void setLeftHipFb(qint32 left_hip_fb);

    qint32 getLeftKneeFlex();
    void setLeftKneeFlex(qint32 left_knee_flex);

    qint32 getLeftAnkleFb();
    void setLeftAnkleFb(qint32 left_ankle_fb);

    qint32 getLeftAnkleUd();
    void setLeftAnkleUd(qint32 left_ankle_ud);

    qint32 getNeckLr();
    void setNeckLr(qint32 neck_lr);


    virtual bool isSet() override;

private:
    void setIsSetFlag(const QJsonObject& obj); //added 2020-12-17

private:
    qint32 right_shoulder_roll;
    bool m_right_shoulder_roll_isSet;

    qint32 right_shoulder_flex;
    bool m_right_shoulder_flex_isSet;

    qint32 right_elbow_flex;
    bool m_right_elbow_flex_isSet;

    qint32 left_shoulder_roll;
    bool m_left_shoulder_roll_isSet;

    qint32 left_shoulder_flex;
    bool m_left_shoulder_flex_isSet;

    qint32 left_elbow_flex;
    bool m_left_elbow_flex_isSet;

    qint32 right_hip_lr;
    bool m_right_hip_lr_isSet;

    qint32 right_hip_fb;
    bool m_right_hip_fb_isSet;

    qint32 right_knee_flex;
    bool m_right_knee_flex_isSet;

    qint32 right_ankle_fb;
    bool m_right_ankle_fb_isSet;

    qint32 right_ankle_ud;
    bool m_right_ankle_ud_isSet;

    qint32 left_hip_lr;
    bool m_left_hip_lr_isSet;

    qint32 left_hip_fb;
    bool m_left_hip_fb_isSet;

    qint32 left_knee_flex;
    bool m_left_knee_flex_isSet;

    qint32 left_ankle_fb;
    bool m_left_ankle_fb_isSet;

    qint32 left_ankle_ud;
    bool m_left_ankle_ud_isSet;

    qint32 neck_lr;
    bool m_neck_lr_isSet;

};

}

#endif /* SWGServosAngles_H_ */
