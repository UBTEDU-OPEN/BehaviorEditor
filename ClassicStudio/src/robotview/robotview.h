#ifndef ROBOTVIEW_H
#define ROBOTVIEW_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <robotview_global.h>
#include<QMenu>
#include <robotcfg.h>
#include <QMutex>
#include <QMutexLocker>

class Fbx;
class ROBOTVIEWSHARED_EXPORT QRobotView : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    QRobotView(QWidget* parent = Q_NULLPTR);
    virtual ~QRobotView ();

    enum CameraPerspective
    {
        CAMERA_PERSPECT = 1,
        CAMERA_FRONT,
        CAMERA_BACK,
        CAMERA_TOP,
        CAMERA_BOTTOM,
        CAMERA_LEFT,
        CAMERA_RIGHT,
        CAMERA_RIGHT_45,
    };

    enum EM_STANCE_TYPE
    {
        EM_STANCE_UNKNOWN = 0,
        EM_STANCE_STAND,
        EM_STANCE_SQUAT,
        EM_STANCE_SPREAD_ARMS,
    };

signals:
    /**
     * @brief sigServoAngleChange  舵机角度变化时发送该信号
     * @param nId
     * @param dAngle
     */
    void sigServoAngleChange (int nId, double dAngle);

    /**
     * @brief sigCurrentSelectServoId  3D模型上鼠标选择节点后发送该节点的舵机ID和所属区域信号
     * @param nId
     * @param nBody
     */
    void sigCurrentSelectServoId (int nId, int nBody);


public slots:
    /**
     * @brief setServoAngle  单个舵机设置角度（带均速运动效果）,默认设置节点选中状态
     * @param nId            舵机ID
     * @param dTargetAngle   舵机目标角度
     * @param runTime        舵机运行至目标角度所花费的时间
     */
    void setServoAngle (int nId, double dTargetAngle, int runTime);

    /**
     * @brief setServoAngle  单个舵机设置角度（带均速运动效果）
     * @param nId            舵机ID
     * @param dTargetAngle   舵机目标角度
     * @param runTime        舵机运行至目标角度所花费的时间
     * @param bSelectNode    舵机ID对应的节点是否设置被选中状态
     */
    void setServoAngle (int nId, double dTargetAngle, int runTime, bool bSelectNode);

    /**
     * @brief setServoAngle  批量舵机设置角度
     * @param servoInfo      多组舵机ID与舵机角度
     */
    void setServoAngle (QMap<int, QPair<double, int>> servoInfo);

    /**
     * @brief setServoAngle  单个舵机设置角度,默认设置节点选中状态
     * @param nId            舵机ID
     * @param dTargetAngle   舵机目标角度
     */
    void setServoAngle (int nId, double dTargetAngle);

    /**
     * @brief setServoAngle  单个舵机设置角度
     * @param nId            舵机ID
     * @param dTargetAngle   舵机目标角度
     * @param bSelectNode    舵机ID对应的节点是否设置被选中状态
     */
    void setServoAngle (int nId, double dTargetAngle, bool bSelectNode);

    /**
     * @brief getServoAngle 获得舵机获取角度
     * @param nId           舵机ID
     * @param dAngle        舵机当前角度
     * @return
     */
    bool getServoAngle (int nId, double &dAngle);

    /**
     * @brief getServosAngle 获取所有舵机角度
     * @param servos
     * @return
     */
    bool getServosAngle(QMap<int,double> &servos);

    /**
     * @brief resetAngle  恢复舵机角度至默认值
     */
    void resetAngle ();

    /**
     * @brief switchWatch  切换视角
     * @param dir          切角类型(CameraPerspective)
     */
    void switchWatch (CameraPerspective dir);

    /**
     * @brief switchStance  切换姿态
     * @param type          姿态类型(EM_STANCE_TYPE)
     */
    void switchStance (EM_STANCE_TYPE type);

public:
    /**
     * @brief registCallBack 注册回调函数
     */
    void registCallBack ();
    /**
     * @brief onServoAngleChange 舵机角度变化事件
     * @param nodeName 舵机在3D模型中的名称(字符串)
     * @param dAngle   舵机变化之后的角度值
     */
    void onServoAngleChange(const char* nodeName, double dAngle);
    /**
     * @brief getServoAxisName 根据舵机在3D模型中的名称获取对应的旋转坐标轴
     * @param nodeName  舵机在3D模型中的名称
     * @return  旋转坐标轴
     */
    std::string getServoAxisName (const char* nodeName);

    std::string getMouseMoveServoAxisName(const char* nodeName);
    /**
     * @brief getServoRotationRange 获取舵机旋转角度范围
     * @param nodeName  舵机在3D模型中的名称
     * @param dMinAngle 舵机旋转最小角度
     * @param dMaxAngle 舵机旋转最大角度
     * @return  成功或者失败
     */
    bool getServoRotationRange (const char* nodeName, double& dMinAngle, double& dMaxAngle);

    void setSelectedNode(int nodeId);
    void stopSetAngle();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

protected:
    void mouseEvent (QMouseEvent *event);
    /**
     * @brief uiToModelAngle 界面角度转化模型角度
     * @param dUiAngle  界面角度
     * @param dOffset   偏移值
     * @return   模型角度
     */
    double uiToModelAngle (double& dUiAngle, double dOffset);

    /**
     * @brief modelToUiAngle 模型角度转界面角度
     * @param dModelAngle   模型角度
     * @param dOffset 偏移值
     * @return  界面角度
     */
    double modelToUiAngle (double& dModelAngle, double dOffset);

    void showMenuWithServoId(int bodyId,int servoId);


private:
    void showMouseCursor(int serovId);
    void getServoIdForNodeName(const char* nodeNome,int *servoId,int *bodyId);
    void moveSelectServo();
    void servoMoveToX(bool isLeft);
    void servoMoveToY(bool isUp);
protected:
    Fbx* m_fbx;
    bool m_bMouseMove;
    bool m_isPressed = false;
    bool m_isSelectNode = false;
    bool m_isMenuSelectNode = false;
    QPoint mouseMovePoint;
    QPoint pressPoint;
    QMenu *mMenu;
    Servo* mSelectServo;
    int mSelectBodyId;
    int mSelectServoId;
    double mCurrentAngle;
    QMap<int,QTimer*> servoTimer;
    QMutex servoTimerMutex;
};

#endif // ROBOTVIEW_H

