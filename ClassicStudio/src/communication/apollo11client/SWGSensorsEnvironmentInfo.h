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

/*
 * SWGSensorsEnvironmentInfo.h
 *
 * 
 */

#ifndef SWGSensorsEnvironmentInfo_H_
#define SWGSensorsEnvironmentInfo_H_

#include <QJsonObject>



#include "SWGObject.h"

namespace Swagger {

class SWGSensorsEnvironmentInfo: public SWGObject {
public:
    SWGSensorsEnvironmentInfo();
    SWGSensorsEnvironmentInfo(QString json);
    ~SWGSensorsEnvironmentInfo();
    void init();
    void cleanup();

    QString asJson () override;
    QJsonObject asJsonObject() override;
    void fromJsonObject(QJsonObject json) override;
    SWGSensorsEnvironmentInfo* fromJson(QString jsonString) override;

    qint32 getId();
    void setId(qint32 id);

    qint32 getSlot();
    void setSlot(qint32 slot);

    qint32 getTemperature();
    void setTemperature(qint32 temperature);

    qint32 getHumidity();
    void setHumidity(qint32 humidity);

    qint32 getPressure();
    void setPressure(qint32 pressure);


    virtual bool isSet() override;

private:
    qint32 id;
    bool m_id_isSet;

    qint32 slot;
    bool m_slot_isSet;

    qint32 temperature;
    bool m_temperature_isSet;

    qint32 humidity;
    bool m_humidity_isSet;

    qint32 pressure;
    bool m_pressure_isSet;

};

}

#endif /* SWGSensorsEnvironmentInfo_H_ */
