#include "robotcfg.h"
#include <QDir>
#include <QDomDocument>
#include <QTextCodec>

Robotcfg::Robotcfg()
    : m_eRobotType(EM_ROBOT_UNKNOWN)
    , servoCount_(0)
{
}

Robotcfg::~Robotcfg ()
{
    qDeleteAll (m_mapBody);
    m_mapBody.clear();
}

Robotcfg* Robotcfg::getInstance ()
{
    static Robotcfg robot;
    return &robot;
}

bool Robotcfg::load(const QString& fileName)
{
    QDir dir;
    if ( !dir.exists(fileName) ) {
        return false;
    }

    QFile file(fileName);
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        return false;
    }
    QByteArray dataArry = file.readAll();
    auto textCodec = QTextCodec::codecForName("UTF8");
    QString strDataArray = textCodec->toUnicode(dataArry);
    file.close();

    QDomDocument dom;
    if ( !dom.setContent( strDataArray ) ) {
        return false;
    }

    QDomElement rootElem = dom.documentElement();
    if ( rootElem.isNull() ) {
        return false;
    }

    m_eRobotType = static_cast<EmRobotType>(rootElem.attribute("type").toInt());
    m_strRobotName = rootElem.attribute("name");
    QDomElement bodyElem = rootElem.firstChildElement("Body");
    for (; !bodyElem.isNull(); bodyElem = bodyElem.nextSiblingElement("Body")) {
        int nId = bodyElem.attribute("id").toInt();
        QString bodyName = bodyElem.attribute("name");
        int bodyType = bodyElem.attribute("type").toInt();
        m_mapBodyIdToType.insert(nId,bodyType);
        Body* pBody = new Body(bodyName,bodyType);

        QDomElement servoElem = bodyElem.firstChildElement("Servo");
        for (; !servoElem.isNull(); servoElem = servoElem.nextSiblingElement("Servo")) {
            int nServoId = servoElem.attribute("id").toInt();
            QString name = servoElem.attribute("name");
            QString engName = servoElem.attribute("apollo_name");
            QString modelName = servoElem.attribute("modelNodeName");
            QString axis = servoElem.attribute("axis");
            double dOffset = servoElem.attribute("offset").toDouble();
            double dInitAngle = servoElem.attribute("init").toDouble();
            double dMinAngle = servoElem.attribute("min").toDouble();
            double dMaxAngle = servoElem.attribute("max").toDouble();
            int oppositeServoId = servoElem.attribute("opposite").toInt();
            m_mapServosInitAngle.insert(nServoId,dInitAngle);
            m_servoApolloNameToId.insert(engName,nServoId);

            Servo* p = new Servo;
            p->setName(name);
            p->setApolloName(engName);
            p->setModelName(modelName);
            p->setAxis(axis);
            p->setOffsetAngle(dOffset);
            p->setInitAngle(dInitAngle);
            p->setMinAngle(dMinAngle);
            p->setMaxAngle(dMaxAngle);
            p->setOppositeServoId(oppositeServoId);
            pBody->addServo(nServoId, p);
            m_mapServo.insert(nServoId,p);
            servoCount_++;
        }

        addBody(nId, pBody);
    }
    return true;
}

void Robotcfg::delBody (int id)
{
    auto iter = m_mapBody.find(id);
    if ( iter != m_mapBody.end() ) {
        delete iter.value();
        m_mapBody.erase(iter);
    }
}

Body* Robotcfg::queryBody (int id)
{
    auto iter = m_mapBody.find(id);
    if ( iter != m_mapBody.end() ) {
        return iter.value();
    }
    return nullptr;
}

Servo* Robotcfg::queryServo (int id)
{
    auto iter = m_mapBody.begin();
    for ( ; iter != m_mapBody.end(); iter ++ ) {
        Body* p = iter.value();
        Servo* pServo = p->queryServo(id);
        if ( pServo ) {
            return pServo;
        }
    }
    return nullptr;
}

int Robotcfg::queryBodyIdByServoId(int servoId)
{
    auto iter = m_mapBody.begin();
    for ( ; iter != m_mapBody.end(); iter ++ ) {
        Body* p = iter.value();
        Servo* pServo = p->queryServo(servoId);
        if ( pServo ) {
            return iter.key();
        }
    }
    return 0;
}

int Robotcfg::queryBodyIdByBodyType(int type)
{
    return m_mapBodyIdToType.key(type,0);
}

Servo *Robotcfg::getServoFromId(int id)
{
    auto it = m_mapServo.begin();
    for(auto it = m_mapServo.begin();it != m_mapServo.end();it++){
        int servoId = it.key();
        auto servo = it.value();
        if(servoId == id){
            return servo;
        }
    }
}

void Robotcfg::calcOppositeServoValue(int& servoId, double& servoAngle)
{
    auto servo = queryServo(servoId);
    servoId = servo->getOppositeServoId();
    double deltaAngle = servoAngle - servo->getInitAngle();
    if(servoId < 0) {
        deltaAngle = -deltaAngle;
        servoId = -servoId;
    }
    auto oppositeServo = queryServo(servoId);
    servoAngle = oppositeServo->getInitAngle() + deltaAngle;
}
