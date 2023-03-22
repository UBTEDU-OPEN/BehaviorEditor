#ifndef ROBOTCFG_H
#define ROBOTCFG_H

#include "robotcfg_global.h"
#include <QStringList>
#include <QMap>

#define RobotConfig Robotcfg::getInstance()

class ROBOTCFGSHARED_EXPORT Servo
{
public:
    Servo () {}
    ~Servo() {}

    enum EmRoateAxisType
    {
        EM_ROATE_AXIS_X = 1,     /**< 绕X轴旋转 */
        EM_ROATE_AXIS_Y,         /**< 绕Y轴旋转 */
        EM_ROATE_AXIS_Z,         /**< 绕Z轴旋转 */
    };

public:
    void setName (const QString& name) { m_strName = name; }
    QString getName () const           { return m_strName; }

    void setApolloName (const QString& name) { m_strApolloName = name; }
    QString getApolloName () const           { return m_strApolloName; }

    void setModelName (const QString& modelName) { m_strModelName = modelName; }
    QString getModelName () const                { return m_strModelName; }

    void setAxis (const QString& strAxis) {
        QString str = strAxis;
        QStringList list = str.split(",", QString::SkipEmptyParts);
        if ( list.count() != 3 ) {
            return ;
        }

        if ( list.at(0) != "0" ) {
            m_eRoateAxis = EM_ROATE_AXIS_X;
            m_nRoateDir = list.at(0).toInt();
        } else if ( list.at(1) != "0" ) {
            m_eRoateAxis = EM_ROATE_AXIS_Y;
            m_nRoateDir = list.at(1).toInt();
        } else if ( list.at(2) != "0" ) {
            m_eRoateAxis = EM_ROATE_AXIS_Z;
            m_nRoateDir = list.at(2).toInt();
        }
    }

    QString getAxis () const {
        QString strAxis = "";
        switch (m_eRoateAxis) {
        case EM_ROATE_AXIS_X:
            strAxis = QString("(%1,0,0)").arg(m_nRoateDir);
            break;
        case EM_ROATE_AXIS_Y:
            strAxis = QString("(0,%1,0)").arg(m_nRoateDir);
            break;
        case EM_ROATE_AXIS_Z:
            strAxis = QString("(0,0,%1)").arg(m_nRoateDir);
            break;
        default:
            break;
        }
        return strAxis;
    }

    EmRoateAxisType getRoateAxis () const { return m_eRoateAxis; }
    int getRoateDir () const              { return m_nRoateDir; }

    void setOffsetAngle (double dValue) { m_dOffsetAngle = dValue; }
    double getOffsetAngle () const      { return m_dOffsetAngle; }

    void setInitAngle (double dValue) { m_dInitAngle = dValue; }
    double getInitAngle () const      { return m_dInitAngle; }

    void setMinAngle (double dValue) { m_dMinAngle = dValue; }
    double getMinAngle () const      { return m_dMinAngle; }

    void setMaxAngle (double dValue) { m_dMaxAngle = dValue; }
    double getMaxAngle () const      { return m_dMaxAngle; }

    void setOppositeServoId(int oppositeServoId) { m_nOppositeServoId = oppositeServoId; }
    int getOppositeServoId() { return m_nOppositeServoId; }

private:
    QString m_strName{""};
    QString m_strApolloName{""};
    QString m_strModelName{""};
    EmRoateAxisType m_eRoateAxis{EM_ROATE_AXIS_X};
    int m_nRoateDir{0};
    double m_dOffsetAngle{0.f};
    double m_dInitAngle{0.f};
    double m_dMinAngle{0.f};
    double m_dMaxAngle{0.f};
    int m_nOppositeServoId{0};
};

class ROBOTCFGSHARED_EXPORT Body
{
public:
    Body () {
    }

    explicit Body (const QString& name, int type) : m_strName(name), m_bodyType(type) {
        m_mapServos.clear();
    }

    ~Body () {
        qDeleteAll(m_mapServos);
        m_mapServos.clear();
    }

public:
    void setName (const QString& name) { m_strName = name; }
    QString getName () const           { return m_strName; }

    void setType(int type) { m_bodyType = type; }
    int getType() { return m_bodyType; }

    void addServo (int id, Servo* servo) {
        m_mapServos[id] = servo;
        m_mapServoNames.push_back(servo->getApolloName());
    }

    void delServo (int id) {
        auto iter = m_mapServos.find(id);
        if ( iter != m_mapServos.end() ) {
            delete iter.value();
            m_mapServos.erase(iter);
        }
    }

    Servo* queryServo (int id) {
        auto iter = m_mapServos.find(id);
        if ( iter != m_mapServos.end() ) {
            return iter.value();
        }
        return nullptr;
    }

    const QMap<int, Servo*>& getServos(){
        return m_mapServos;
    }

    QList<QString> getServoNames() { return m_mapServoNames; }

private:
    QString m_strName{""};
    int m_bodyType{0};
    QMap<int, Servo*>  m_mapServos;
    QList<QString> m_mapServoNames;
};

class ROBOTCFGSHARED_EXPORT Robotcfg
{
public:
    static Robotcfg* getInstance ();

    enum EmRobotType
    {
        EM_ROBOT_UNKNOWN = 0,
        EM_ROBOT_CLASSIC = 1,   /**< classic机器人 */
        EM_ROBOT_YANSHEE11 = 2,
        EM_ROBOT_YANSHEE15 = 3,
        EM_ROBOT_ALPHAMINI = 4
    };

    enum EmAddType
    {
        EM_ADD_ALL = 0,
        EM_ADD_BODY = 1,
        EM_ADD_SERVO = 2
    };

    enum EmRobotBody
    {
        EM_BODY_UNKNOWN = 0,
        EM_BODY_HEAD,           /**< 头部 */
        EM_BODY_RIGHT_HAND,     /**< 右手 */
        EM_BODY_LEFT_HAND,      /**< 左手 */
        EM_BODY_RIGHT_LEG,      /**< 右腿 */
        EM_BODY_LEFT_LEG,       /**< 左腿 */
        EM_BODY_ALL
    };

public:
    bool load(const QString& fileName);

public:
    void setRobotType (EmRobotType eRobotType) { m_eRobotType = eRobotType; }
    EmRobotType getRobotType () const          { return m_eRobotType; }

    void setRobotName (const QString& strName) { m_strRobotName = strName; }
    QString getRobotName () const              { return m_strRobotName; }

    void addBody (int id, Body* body) { m_mapBody[id] = body; }
    void delBody (int id);
    Body* queryBody (int id);
    Servo* queryServo (int id);
    int queryBodyIdByServoId(int servoId);
    int queryBodyIdByBodyType(int type);
    int getServoCount() { return servoCount_; }

    const QMap<int, Body*>& getBodys() {
        return m_mapBody;
    }

    const QMap<int, Servo*>& getServos() { return m_mapServo; }
    Servo* getServoFromId(int id);
    QList<QString> getServoApolloNames() { return m_servoApolloNameToId.keys(); }
    const QMap<int, double>& getServosInitAngle() { return m_mapServosInitAngle; }
    int queryServoIdByApolloName(const QString& name) { return m_servoApolloNameToId.value(name,0); }
    QString queryApolloNameByServoId(int servoId) { return m_servoApolloNameToId.key(servoId,""); }
    void calcOppositeServoValue(int& servoId, double& servoAngle);
    int queryBodyTypeByBodyId(int bodyId) { return m_mapBodyIdToType.key(bodyId,0); }

private:
    Robotcfg();
    ~Robotcfg ();

private:
    EmRobotType m_eRobotType;
    QString     m_strRobotName;
    QMap<int, Body*> m_mapBody;
    QMap<int, Servo*> m_mapServo;
    //id,type
    QMap<int, int> m_mapBodyIdToType;
    QMap<int, double> m_mapServosInitAngle;
    QMap<QString, int> m_servoApolloNameToId;
    int servoCount_;
};

#endif // ROBOTCFG_H
