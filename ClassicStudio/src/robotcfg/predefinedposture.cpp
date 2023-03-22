#include "predefinedposture.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QDomDocument>

PredefinedPosture* PredefinedPosture::s_instance = new PredefinedPosture;

PredefinedPosture::PredefinedPosture()
{
}


bool PredefinedPosture::loadFile(const QString& filePath)
{
    QDir dir;
    if ( !dir.exists(filePath) ) {
        return false;
    }

    QFile file(filePath);
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        return false;
    }
    QByteArray dataArry = file.readAll();
    file.close();

    QDomDocument dom;
    if ( !dom.setContent( dataArry ) ) {
        return false;
    }

    QDomElement rootElem = dom.documentElement();
    if ( rootElem.isNull() ) {
        return false;
    }

    QDomElement postureElem = rootElem.firstChildElement("Posture");
    for (; !postureElem.isNull(); postureElem = postureElem.nextSiblingElement("Posture")) {
        QString postureName = postureElem.attribute("name");
        Posture* posture = new Posture(postureName);

        QDomElement servoElem = postureElem.firstChildElement("Servo");
        for (; !servoElem.isNull(); servoElem = servoElem.nextSiblingElement("Servo")) {
            int servoId = servoElem.attribute("id").toInt();
            double servoValue = servoElem.attribute("value").toDouble();
            posture->addServo(servoId,servoValue);
        }
        m_postures.push_back(posture);
    }
    return true;
}
