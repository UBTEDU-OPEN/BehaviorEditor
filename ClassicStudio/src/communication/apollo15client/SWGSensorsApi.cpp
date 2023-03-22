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

#include "SWGSensorsApi.h"
#include "SWGHelpers.h"
#include "SWGModelFactory.h"
#include "SWGQObjectWrapper.h"

#include <QJsonArray>
#include <QJsonDocument>

namespace Swagger {

SWGSensorsApi::SWGSensorsApi() {}

SWGSensorsApi::~SWGSensorsApi() {}

SWGSensorsApi::SWGSensorsApi(QString host, QString basePath) {
    this->host = host;
    this->basePath = basePath;
}

void
SWGSensorsApi::getSensorsEnvironment(QList<qint32>* slot) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sensors/environment");




    if (slot->size() > 0) {
      if (QString("multi").indexOf("multi") == 0) {
        foreach(qint32 t, *slot) {
          if (fullPath.indexOf("?") > 0)
            fullPath.append("&");
          else
            fullPath.append("?");
          fullPath.append("slot=").append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("ssv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("slot=");
        qint32 count = 0;
        foreach(qint32 t, *slot) {
          if (count > 0) {
            fullPath.append(" ");
          }
          fullPath.append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("tsv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("slot=");
        qint32 count = 0;
        foreach(qint32 t, *slot) {
          if (count > 0) {
            fullPath.append("\t");
          }
          fullPath.append(stringValue(t));
        }
      }
    }


    SWGHttpRequestWorker *worker = new SWGHttpRequestWorker();
    SWGHttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &SWGHttpRequestWorker::on_execution_finished,
            this,
            &SWGSensorsApi::getSensorsEnvironmentCallback);

    worker->execute(&input);
}

void
SWGSensorsApi::getSensorsEnvironmentCallback(SWGHttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }

    QString json(worker->response);
    SWGSensorsEnvironmentValueResponse* output = static_cast<SWGSensorsEnvironmentValueResponse*>(create(json, QString("SWGSensorsEnvironmentValueResponse")));
    auto wrapper = new SWGQObjectWrapper<SWGSensorsEnvironmentValueResponse*> (output);
    wrapper->deleteLater();
    worker->deleteLater();

    if (worker->error_type == QNetworkReply::NoError) {
        emit getSensorsEnvironmentSignal(output);
    } else {
        emit getSensorsEnvironmentSignalE(output, error_type, error_str);
        emit getSensorsEnvironmentSignalEFull(worker, error_type, error_str);
    }
}

void
SWGSensorsApi::getSensorsGyro() {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sensors/gyro");



    SWGHttpRequestWorker *worker = new SWGHttpRequestWorker();
    SWGHttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &SWGHttpRequestWorker::on_execution_finished,
            this,
            &SWGSensorsApi::getSensorsGyroCallback);

    worker->execute(&input);
}

void
SWGSensorsApi::getSensorsGyroCallback(SWGHttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }

    QString json(worker->response);
    SWGSensorsGyroValueResponse* output = static_cast<SWGSensorsGyroValueResponse*>(create(json, QString("SWGSensorsGyroValueResponse")));
    auto wrapper = new SWGQObjectWrapper<SWGSensorsGyroValueResponse*> (output);
    wrapper->deleteLater();
    worker->deleteLater();

    if (worker->error_type == QNetworkReply::NoError) {
        emit getSensorsGyroSignal(output);
    } else {
        emit getSensorsGyroSignalE(output, error_type, error_str);
        emit getSensorsGyroSignalEFull(worker, error_type, error_str);
    }
}

void
SWGSensorsApi::getSensorsInfrared(QList<qint32>* id, QList<qint32>* slot) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sensors/infrared");




    if (id->size() > 0) {
      if (QString("multi").indexOf("multi") == 0) {
        foreach(qint32 t, *id) {
          if (fullPath.indexOf("?") > 0)
            fullPath.append("&");
          else
            fullPath.append("?");
          fullPath.append("id=").append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("ssv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("id=");
        qint32 count = 0;
        foreach(qint32 t, *id) {
          if (count > 0) {
            fullPath.append(" ");
          }
          fullPath.append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("tsv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("id=");
        qint32 count = 0;
        foreach(qint32 t, *id) {
          if (count > 0) {
            fullPath.append("\t");
          }
          fullPath.append(stringValue(t));
        }
      }
    }



    if (slot->size() > 0) {
      if (QString("multi").indexOf("multi") == 0) {
        foreach(qint32 t, *slot) {
          if (fullPath.indexOf("?") > 0)
            fullPath.append("&");
          else
            fullPath.append("?");
          fullPath.append("slot=").append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("ssv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("slot=");
        qint32 count = 0;
        foreach(qint32 t, *slot) {
          if (count > 0) {
            fullPath.append(" ");
          }
          fullPath.append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("tsv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("slot=");
        qint32 count = 0;
        foreach(qint32 t, *slot) {
          if (count > 0) {
            fullPath.append("\t");
          }
          fullPath.append(stringValue(t));
        }
      }
    }


    SWGHttpRequestWorker *worker = new SWGHttpRequestWorker();
    SWGHttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &SWGHttpRequestWorker::on_execution_finished,
            this,
            &SWGSensorsApi::getSensorsInfraredCallback);

    worker->execute(&input);
}

void
SWGSensorsApi::getSensorsInfraredCallback(SWGHttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }

    QString json(worker->response);
    SWGSensorsInfraredValueResponse* output = static_cast<SWGSensorsInfraredValueResponse*>(create(json, QString("SWGSensorsInfraredValueResponse")));
    auto wrapper = new SWGQObjectWrapper<SWGSensorsInfraredValueResponse*> (output);
    wrapper->deleteLater();
    worker->deleteLater();

    if (worker->error_type == QNetworkReply::NoError) {
        emit getSensorsInfraredSignal(output);
    } else {
        emit getSensorsInfraredSignalE(output, error_type, error_str);
        emit getSensorsInfraredSignalEFull(worker, error_type, error_str);
    }
}

void
SWGSensorsApi::getSensorsList() {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sensors/list");



    SWGHttpRequestWorker *worker = new SWGHttpRequestWorker();
    SWGHttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &SWGHttpRequestWorker::on_execution_finished,
            this,
            &SWGSensorsApi::getSensorsListCallback);

    worker->execute(&input);
}

void
SWGSensorsApi::getSensorsListCallback(SWGHttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }

    QString json(worker->response);
    SWGSensorsListResponse* output = static_cast<SWGSensorsListResponse*>(create(json, QString("SWGSensorsListResponse")));
    auto wrapper = new SWGQObjectWrapper<SWGSensorsListResponse*> (output);
    wrapper->deleteLater();
    worker->deleteLater();

    if (worker->error_type == QNetworkReply::NoError) {
        emit getSensorsListSignal(output);
    } else {
        emit getSensorsListSignalE(output, error_type, error_str);
        emit getSensorsListSignalEFull(worker, error_type, error_str);
    }
}

void
SWGSensorsApi::getSensorsPressure(QList<qint32>* id, QList<qint32>* slot) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sensors/pressure");




    if (id->size() > 0) {
      if (QString("multi").indexOf("multi") == 0) {
        foreach(qint32 t, *id) {
          if (fullPath.indexOf("?") > 0)
            fullPath.append("&");
          else
            fullPath.append("?");
          fullPath.append("id=").append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("ssv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("id=");
        qint32 count = 0;
        foreach(qint32 t, *id) {
          if (count > 0) {
            fullPath.append(" ");
          }
          fullPath.append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("tsv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("id=");
        qint32 count = 0;
        foreach(qint32 t, *id) {
          if (count > 0) {
            fullPath.append("\t");
          }
          fullPath.append(stringValue(t));
        }
      }
    }



    if (slot->size() > 0) {
      if (QString("multi").indexOf("multi") == 0) {
        foreach(qint32 t, *slot) {
          if (fullPath.indexOf("?") > 0)
            fullPath.append("&");
          else
            fullPath.append("?");
          fullPath.append("slot=").append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("ssv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("slot=");
        qint32 count = 0;
        foreach(qint32 t, *slot) {
          if (count > 0) {
            fullPath.append(" ");
          }
          fullPath.append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("tsv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("slot=");
        qint32 count = 0;
        foreach(qint32 t, *slot) {
          if (count > 0) {
            fullPath.append("\t");
          }
          fullPath.append(stringValue(t));
        }
      }
    }


    SWGHttpRequestWorker *worker = new SWGHttpRequestWorker();
    SWGHttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &SWGHttpRequestWorker::on_execution_finished,
            this,
            &SWGSensorsApi::getSensorsPressureCallback);

    worker->execute(&input);
}

void
SWGSensorsApi::getSensorsPressureCallback(SWGHttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }

    QString json(worker->response);
    SWGSensorsPressureValueResponse* output = static_cast<SWGSensorsPressureValueResponse*>(create(json, QString("SWGSensorsPressureValueResponse")));
    auto wrapper = new SWGQObjectWrapper<SWGSensorsPressureValueResponse*> (output);
    wrapper->deleteLater();
    worker->deleteLater();

    if (worker->error_type == QNetworkReply::NoError) {
        emit getSensorsPressureSignal(output);
    } else {
        emit getSensorsPressureSignalE(output, error_type, error_str);
        emit getSensorsPressureSignalEFull(worker, error_type, error_str);
    }
}

void
SWGSensorsApi::getSensorsTouch(QList<qint32>* id, QList<qint32>* slot) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sensors/touch");




    if (id->size() > 0) {
      if (QString("multi").indexOf("multi") == 0) {
        foreach(qint32 t, *id) {
          if (fullPath.indexOf("?") > 0)
            fullPath.append("&");
          else
            fullPath.append("?");
          fullPath.append("id=").append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("ssv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("id=");
        qint32 count = 0;
        foreach(qint32 t, *id) {
          if (count > 0) {
            fullPath.append(" ");
          }
          fullPath.append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("tsv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("id=");
        qint32 count = 0;
        foreach(qint32 t, *id) {
          if (count > 0) {
            fullPath.append("\t");
          }
          fullPath.append(stringValue(t));
        }
      }
    }



    if (slot->size() > 0) {
      if (QString("multi").indexOf("multi") == 0) {
        foreach(qint32 t, *slot) {
          if (fullPath.indexOf("?") > 0)
            fullPath.append("&");
          else
            fullPath.append("?");
          fullPath.append("slot=").append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("ssv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("slot=");
        qint32 count = 0;
        foreach(qint32 t, *slot) {
          if (count > 0) {
            fullPath.append(" ");
          }
          fullPath.append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("tsv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("slot=");
        qint32 count = 0;
        foreach(qint32 t, *slot) {
          if (count > 0) {
            fullPath.append("\t");
          }
          fullPath.append(stringValue(t));
        }
      }
    }


    SWGHttpRequestWorker *worker = new SWGHttpRequestWorker();
    SWGHttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &SWGHttpRequestWorker::on_execution_finished,
            this,
            &SWGSensorsApi::getSensorsTouchCallback);

    worker->execute(&input);
}

void
SWGSensorsApi::getSensorsTouchCallback(SWGHttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }

    QString json(worker->response);
    SWGSensorsTouchValueResponse* output = static_cast<SWGSensorsTouchValueResponse*>(create(json, QString("SWGSensorsTouchValueResponse")));
    auto wrapper = new SWGQObjectWrapper<SWGSensorsTouchValueResponse*> (output);
    wrapper->deleteLater();
    worker->deleteLater();

    if (worker->error_type == QNetworkReply::NoError) {
        emit getSensorsTouchSignal(output);
    } else {
        emit getSensorsTouchSignalE(output, error_type, error_str);
        emit getSensorsTouchSignalEFull(worker, error_type, error_str);
    }
}

void
SWGSensorsApi::getSensorsUltrasonic(QList<qint32>* id, QList<qint32>* slot) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sensors/ultrasonic");




    if (id->size() > 0) {
      if (QString("multi").indexOf("multi") == 0) {
        foreach(qint32 t, *id) {
          if (fullPath.indexOf("?") > 0)
            fullPath.append("&");
          else
            fullPath.append("?");
          fullPath.append("id=").append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("ssv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("id=");
        qint32 count = 0;
        foreach(qint32 t, *id) {
          if (count > 0) {
            fullPath.append(" ");
          }
          fullPath.append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("tsv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("id=");
        qint32 count = 0;
        foreach(qint32 t, *id) {
          if (count > 0) {
            fullPath.append("\t");
          }
          fullPath.append(stringValue(t));
        }
      }
    }



    if (slot->size() > 0) {
      if (QString("multi").indexOf("multi") == 0) {
        foreach(qint32 t, *slot) {
          if (fullPath.indexOf("?") > 0)
            fullPath.append("&");
          else
            fullPath.append("?");
          fullPath.append("slot=").append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("ssv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("slot=");
        qint32 count = 0;
        foreach(qint32 t, *slot) {
          if (count > 0) {
            fullPath.append(" ");
          }
          fullPath.append(stringValue(t));
        }
      }
      else if (QString("multi").indexOf("tsv") == 0) {
        if (fullPath.indexOf("?") > 0)
          fullPath.append("&");
        else
          fullPath.append("?");
        fullPath.append("slot=");
        qint32 count = 0;
        foreach(qint32 t, *slot) {
          if (count > 0) {
            fullPath.append("\t");
          }
          fullPath.append(stringValue(t));
        }
      }
    }


    SWGHttpRequestWorker *worker = new SWGHttpRequestWorker();
    SWGHttpRequestInput input(fullPath, "GET");





    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &SWGHttpRequestWorker::on_execution_finished,
            this,
            &SWGSensorsApi::getSensorsUltrasonicCallback);

    worker->execute(&input);
}

void
SWGSensorsApi::getSensorsUltrasonicCallback(SWGHttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }

    QString json(worker->response);
    SWGSensorsUltrasonicValueResponse* output = static_cast<SWGSensorsUltrasonicValueResponse*>(create(json, QString("SWGSensorsUltrasonicValueResponse")));
    auto wrapper = new SWGQObjectWrapper<SWGSensorsUltrasonicValueResponse*> (output);
    wrapper->deleteLater();
    worker->deleteLater();

    if (worker->error_type == QNetworkReply::NoError) {
        emit getSensorsUltrasonicSignal(output);
    } else {
        emit getSensorsUltrasonicSignalE(output, error_type, error_str);
        emit getSensorsUltrasonicSignalEFull(worker, error_type, error_str);
    }
}

void
SWGSensorsApi::putSensors(SWGSensorsOperationRequest& body) {
    QString fullPath;
    fullPath.append(this->host).append(this->basePath).append("/sensors");



    SWGHttpRequestWorker *worker = new SWGHttpRequestWorker();
    SWGHttpRequestInput input(fullPath, "PUT");


    
    QString output = body.asJson();
    input.request_body.append(output);
    


    foreach(QString key, this->defaultHeaders.keys()) {
        input.headers.insert(key, this->defaultHeaders.value(key));
    }

    connect(worker,
            &SWGHttpRequestWorker::on_execution_finished,
            this,
            &SWGSensorsApi::putSensorsCallback);

    worker->execute(&input);
}

void
SWGSensorsApi::putSensorsCallback(SWGHttpRequestWorker * worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    }
    else {
        msg = "Error: " + worker->error_str;
    }

    QString json(worker->response);
    SWGCommonResponse* output = static_cast<SWGCommonResponse*>(create(json, QString("SWGCommonResponse")));
    auto wrapper = new SWGQObjectWrapper<SWGCommonResponse*> (output);
    wrapper->deleteLater();
    worker->deleteLater();

    if (worker->error_type == QNetworkReply::NoError) {
        emit putSensorsSignal(output);
    } else {
        emit putSensorsSignalE(output, error_type, error_str);
        emit putSensorsSignalEFull(worker, error_type, error_str);
    }
}


}