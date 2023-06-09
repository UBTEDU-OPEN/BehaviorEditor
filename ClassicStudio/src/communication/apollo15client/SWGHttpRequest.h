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

/**
 * Based on http://www.creativepulse.gr/en/blog/2014/restful-api-requests-using-qt-cpp-for-linux-mac-osx-ms-windows
 * By Alex Stylianos
 *
 **/

#ifndef SWG_HTTPREQUESTWORKER_H
#define SWG_HTTPREQUESTWORKER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>



namespace Swagger {

enum SWGHttpRequestVarLayout {NOT_SET, ADDRESS, URL_ENCODED, MULTIPART};

class SWGHttpRequestInputFileElement {

public:
    QString variable_name;
    QString local_filename;
    QString request_filename;
    QString mime_type;

};


class SWGHttpRequestInput {

public:
    QString url_str;
    QString http_method;
    SWGHttpRequestVarLayout var_layout;
    QMap<QString, QString> vars;
    QMap<QString, QString> headers;
    QList<SWGHttpRequestInputFileElement> files;
    QByteArray request_body;

    SWGHttpRequestInput();
    SWGHttpRequestInput(QString v_url_str, QString v_http_method);
    void initialize();
    void add_var(QString key, QString value);
    void add_file(QString variable_name, QString local_filename, QString request_filename, QString mime_type);

};


class SWGHttpRequestWorker : public QObject {
    Q_OBJECT

public:
    QByteArray response;
    QNetworkReply::NetworkError error_type;
    QString error_str;

    explicit SWGHttpRequestWorker(QObject *parent = 0);
    virtual ~SWGHttpRequestWorker();

    QString http_attribute_encode(QString attribute_name, QString input);
    void execute(SWGHttpRequestInput *input);
    static QSslConfiguration* sslDefaultConfiguration;

signals:
    void on_execution_finished(SWGHttpRequestWorker *worker);

private:
    QNetworkAccessManager *manager;

private slots:
    void on_manager_finished(QNetworkReply *reply);

};

}

#endif // SWG_HTTPREQUESTWORKER_H
