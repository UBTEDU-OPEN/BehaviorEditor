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
 * SWGVoicePutOfflineSyntax.h
 *
 * 
 */

#ifndef SWGVoicePutOfflineSyntax_H_
#define SWGVoicePutOfflineSyntax_H_

#include <QJsonObject>


#include "SWGVoiceOfflineSlot.h"
#include "SWGVoiceOfflineSyntaxRule.h"
#include <QList>
#include <QString>

#include "SWGObject.h"

namespace Swagger {

class SWGVoicePutOfflineSyntax: public SWGObject {
public:
    SWGVoicePutOfflineSyntax();
    SWGVoicePutOfflineSyntax(QString json);
    ~SWGVoicePutOfflineSyntax();
    void init();
    void cleanup();

    QString asJson () override;
    QJsonObject asJsonObject() override;
    void fromJsonObject(QJsonObject json) override;
    SWGVoicePutOfflineSyntax* fromJson(QString jsonString) override;

    QString* getGrammar();
    void setGrammar(QString* grammar);

    QList<SWGVoiceOfflineSlot*>* getSlot();
    void setSlot(QList<SWGVoiceOfflineSlot*>* slot);

    QString* getStart();
    void setStart(QString* start);

    QString* getStartinfo();
    void setStartinfo(QString* startinfo);

    QList<SWGVoiceOfflineSyntaxRule*>* getRule();
    void setRule(QList<SWGVoiceOfflineSyntaxRule*>* rule);


    virtual bool isSet() override;

private:
    QString* grammar;
    bool m_grammar_isSet;

    QList<SWGVoiceOfflineSlot*>* slot;
    bool m_slot_isSet;

    QString* start;
    bool m_start_isSet;

    QString* startinfo;
    bool m_startinfo_isSet;

    QList<SWGVoiceOfflineSyntaxRule*>* rule;
    bool m_rule_isSet;

};

}

#endif /* SWGVoicePutOfflineSyntax_H_ */
