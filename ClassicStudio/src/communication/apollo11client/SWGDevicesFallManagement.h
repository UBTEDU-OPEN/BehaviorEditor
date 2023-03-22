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
 * SWGDevicesFallManagement.h
 *
 * 
 */

#ifndef SWGDevicesFallManagement_H_
#define SWGDevicesFallManagement_H_

#include <QJsonObject>



#include "SWGObject.h"

namespace Swagger {

class SWGDevicesFallManagement: public SWGObject {
public:
    SWGDevicesFallManagement();
    SWGDevicesFallManagement(QString json);
    ~SWGDevicesFallManagement();
    void init();
    void cleanup();

    QString asJson () override;
    QJsonObject asJsonObject() override;
    void fromJsonObject(QJsonObject json) override;
    SWGDevicesFallManagement* fromJson(QString jsonString) override;

    bool isEnable();
    void setEnable(bool enable);


    virtual bool isSet() override;

private:
    bool enable;
    bool m_enable_isSet;

};

}

#endif /* SWGDevicesFallManagement_H_ */