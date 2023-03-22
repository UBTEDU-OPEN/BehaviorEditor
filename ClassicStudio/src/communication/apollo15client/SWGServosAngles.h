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

    qint32 getHeadPitch();
    void setHeadPitch(qint32 head_pitch);

    qint32 getHeadYaw();
    void setHeadYaw(qint32 head_yaw);

    qint32 getRShoulderPitch();
    void setRShoulderPitch(qint32 r_shoulder_pitch);

    qint32 getRShoulderRoll();
    void setRShoulderRoll(qint32 r_shoulder_roll);

    qint32 getRElbowRoll();
    void setRElbowRoll(qint32 r_elbow_roll);

    qint32 getRHand();
    void setRHand(qint32 r_hand);

    qint32 getLShoulderPitch();
    void setLShoulderPitch(qint32 l_shoulder_pitch);

    qint32 getLShoulderRoll();
    void setLShoulderRoll(qint32 l_shoulder_roll);

    qint32 getLElbowRoll();
    void setLElbowRoll(qint32 l_elbow_roll);

    qint32 getLHand();
    void setLHand(qint32 l_hand);

    qint32 getRHipYaw();
    void setRHipYaw(qint32 r_hip_yaw);

    qint32 getRHipRoll();
    void setRHipRoll(qint32 r_hip_roll);

    qint32 getRHipPitch();
    void setRHipPitch(qint32 r_hip_pitch);

    qint32 getRKneePitch();
    void setRKneePitch(qint32 r_knee_pitch);

    qint32 getRAnklePitch();
    void setRAnklePitch(qint32 r_ankle_pitch);

    qint32 getRAnkleRoll();
    void setRAnkleRoll(qint32 r_ankle_roll);

    qint32 getLHipYaw();
    void setLHipYaw(qint32 l_hip_yaw);

    qint32 getLHipRoll();
    void setLHipRoll(qint32 l_hip_roll);

    qint32 getLHipPitch();
    void setLHipPitch(qint32 l_hip_pitch);

    qint32 getLKneePitch();
    void setLKneePitch(qint32 l_knee_pitch);

    qint32 getLAnklePitch();
    void setLAnklePitch(qint32 l_ankle_pitch);

    qint32 getLAnkleRoll();
    void setLAnkleRoll(qint32 l_ankle_roll);


    virtual bool isSet() override;

private:
    void setIsSetFlag(const QJsonObject& obj); //added 2020-12-17


private:
    qint32 head_pitch;
    bool m_head_pitch_isSet;

    qint32 head_yaw;
    bool m_head_yaw_isSet;

    qint32 r_shoulder_pitch;
    bool m_r_shoulder_pitch_isSet;

    qint32 r_shoulder_roll;
    bool m_r_shoulder_roll_isSet;

    qint32 r_elbow_roll;
    bool m_r_elbow_roll_isSet;

    qint32 r_hand;
    bool m_r_hand_isSet;

    qint32 l_shoulder_pitch;
    bool m_l_shoulder_pitch_isSet;

    qint32 l_shoulder_roll;
    bool m_l_shoulder_roll_isSet;

    qint32 l_elbow_roll;
    bool m_l_elbow_roll_isSet;

    qint32 l_hand;
    bool m_l_hand_isSet;

    qint32 r_hip_yaw;
    bool m_r_hip_yaw_isSet;

    qint32 r_hip_roll;
    bool m_r_hip_roll_isSet;

    qint32 r_hip_pitch;
    bool m_r_hip_pitch_isSet;

    qint32 r_knee_pitch;
    bool m_r_knee_pitch_isSet;

    qint32 r_ankle_pitch;
    bool m_r_ankle_pitch_isSet;

    qint32 r_ankle_roll;
    bool m_r_ankle_roll_isSet;

    qint32 l_hip_yaw;
    bool m_l_hip_yaw_isSet;

    qint32 l_hip_roll;
    bool m_l_hip_roll_isSet;

    qint32 l_hip_pitch;
    bool m_l_hip_pitch_isSet;

    qint32 l_knee_pitch;
    bool m_l_knee_pitch_isSet;

    qint32 l_ankle_pitch;
    bool m_l_ankle_pitch_isSet;

    qint32 l_ankle_roll;
    bool m_l_ankle_roll_isSet;

};

}

#endif /* SWGServosAngles_H_ */