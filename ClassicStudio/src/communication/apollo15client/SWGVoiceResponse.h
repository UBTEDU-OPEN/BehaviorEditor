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
 * SWGVoiceResponse.h
 *
 *  语音识别结果 
 */

#ifndef SWGVoiceResponse_H_
#define SWGVoiceResponse_H_

#include <QJsonObject>


#include "SWGObject.h"
#include <QString>

#include "SWGObject.h"

namespace Swagger {

class SWGVoiceResponse: public SWGObject {
public:
    SWGVoiceResponse();
    SWGVoiceResponse(QString json);
    ~SWGVoiceResponse();
    void init();
    void cleanup();

    QString asJson () override;
    QJsonObject asJsonObject() override;
    void fromJsonObject(QJsonObject json) override;
    SWGVoiceResponse* fromJson(QString jsonString) override;

    qint32 getCode();
    void setCode(qint32 code);

    QString* getType();
    void setType(QString* type);

    QString* getStatus();
    void setStatus(QString* status);

    SWGObject* getData();
    void setData(SWGObject* data);

    qint64 getTimestamp();
    void setTimestamp(qint64 timestamp);

    QString* getMsg();
    void setMsg(QString* msg);


    virtual bool isSet() override;

private:
    qint32 code;
    bool m_code_isSet;

    QString* type;
    bool m_type_isSet;

    QString* status;
    bool m_status_isSet;

    SWGObject* data;
    bool m_data_isSet;

    qint64 timestamp;
    bool m_timestamp_isSet;

    QString* msg;
    bool m_msg_isSet;

};

}

#endif /* SWGVoiceResponse_H_ */