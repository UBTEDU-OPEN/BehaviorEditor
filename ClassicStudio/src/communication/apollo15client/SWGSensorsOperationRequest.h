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
 * SWGSensorsOperationRequest.h
 *
 * 
 */

#ifndef SWGSensorsOperationRequest_H_
#define SWGSensorsOperationRequest_H_

#include <QJsonObject>


#include "SWGSensorsParameter.h"
#include <QString>

#include "SWGObject.h"

namespace Swagger {

class SWGSensorsOperationRequest: public SWGObject {
public:
    SWGSensorsOperationRequest();
    SWGSensorsOperationRequest(QString json);
    ~SWGSensorsOperationRequest();
    void init();
    void cleanup();

    QString asJson () override;
    QJsonObject asJsonObject() override;
    void fromJsonObject(QJsonObject json) override;
    SWGSensorsOperationRequest* fromJson(QString jsonString) override;

    QString* getOperation();
    void setOperation(QString* operation);

    SWGSensorsParameter* getSensor();
    void setSensor(SWGSensorsParameter* sensor);


    virtual bool isSet() override;

private:
    QString* operation;
    bool m_operation_isSet;

    SWGSensorsParameter* sensor;
    bool m_sensor_isSet;

};

}

#endif /* SWGSensorsOperationRequest_H_ */
