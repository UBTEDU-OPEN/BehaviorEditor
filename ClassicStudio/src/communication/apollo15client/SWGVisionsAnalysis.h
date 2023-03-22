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
 * SWGVisionsAnalysis.h
 *
 * 
 */

#ifndef SWGVisionsAnalysis_H_
#define SWGVisionsAnalysis_H_

#include <QJsonObject>


#include <QString>

#include "SWGObject.h"

namespace Swagger {

class SWGVisionsAnalysis: public SWGObject {
public:
    SWGVisionsAnalysis();
    SWGVisionsAnalysis(QString json);
    ~SWGVisionsAnalysis();
    void init();
    void cleanup();

    QString asJson () override;
    QJsonObject asJsonObject() override;
    void fromJsonObject(QJsonObject json) override;
    SWGVisionsAnalysis* fromJson(QString jsonString) override;

    qint32 getAge();
    void setAge(qint32 age);

    QString* getGroup();
    void setGroup(QString* group);

    QString* getGender();
    void setGender(QString* gender);

    QString* getGlass();
    void setGlass(QString* glass);

    QString* getMask();
    void setMask(QString* mask);

    QString* getExpression();
    void setExpression(QString* expression);


    virtual bool isSet() override;

private:
    qint32 age;
    bool m_age_isSet;

    QString* group;
    bool m_group_isSet;

    QString* gender;
    bool m_gender_isSet;

    QString* glass;
    bool m_glass_isSet;

    QString* mask;
    bool m_mask_isSet;

    QString* expression;
    bool m_expression_isSet;

};

}

#endif /* SWGVisionsAnalysis_H_ */
