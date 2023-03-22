#include "robotview.h"
#include <QCoreApplication>
#include "fbx.h"
#include "robotview.h"
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include<datacenter.h>
#include <qmath.h>
#include <QTimer>
#include<QApplication>

#ifdef YANSHEE1_5
#define MODEL_PATH     QString(qApp->applicationDirPath() + "/resource/robot/yanshee1_5.fbx").toUtf8()
#else
#define MODEL_PATH     QString(qApp->applicationDirPath() + "/resource/robot/yanshee1_1.fbx").toUtf8()
#endif
QRobotView::QRobotView(QWidget *parent) :
    QOpenGLWidget (parent), QOpenGLFunctions(),
    m_fbx(nullptr), m_bMouseMove(false),mMenu(nullptr)
{
    setMouseTracking(true);
}

QRobotView::~QRobotView()
{
    makeCurrent();
    if ( m_fbx ){
        delete m_fbx;
        m_fbx = nullptr;
    }
    doneCurrent();
}

void QRobotView::setServoAngle (int nId, double dTargetAngle, int runTime)
{
    setServoAngle(nId, dTargetAngle, runTime, true);
}

void QRobotView::setServoAngle (int nId, double dTargetAngle, int runTime, bool bSelectNode)
{
    /**< 判定给定的舵机ID是否有效 */
    Servo* pServo = RobotConfig->queryServo(nId);
    if ( pServo == nullptr ) {
        return ;
    }

    /**< 判定给定的目标角度是否有效,并调整相对应的目标角度 */
    if ( dTargetAngle < pServo->getMinAngle() ) {
        dTargetAngle = pServo->getMinAngle();
    } else if ( dTargetAngle > pServo->getMaxAngle() ) {
        dTargetAngle = pServo->getMaxAngle();
    }

    /**< 界面角度转化模型角度 */
    uiToModelAngle(dTargetAngle, pServo->getOffsetAngle());

    QString strModelName = pServo->getModelName();
    QStringList modelNameList;
    modelNameList = strModelName.split(",",QString::SplitBehavior::SkipEmptyParts);
    //    qDebug() << "QRobotView::setServoAngle" << modelNameList << modelNameList.size();
    if(modelNameList.size() == 0) {
        return;
    }
    dTargetAngle /= modelNameList.size();
    int i = 1;
    for(QString modelName : modelNameList) {
        std::string sName = modelName.toStdString();
        const char* name = sName.c_str();
        Fbx::AxisType dir = static_cast<Fbx::AxisType>(pServo->getRoateAxis());
        /**< 获取舵机当前角度 */
        double dCurAngle = m_fbx->getServoAngle(name, dir);
        if ( qFuzzyCompare(dCurAngle, dTargetAngle) ) {
            return ;
        }

        /**< 舵机角度方向调整 */
        dTargetAngle *= pServo->getRoateDir();
        if(i == 2) {
            dTargetAngle = -dTargetAngle;
        }

        double delta = fabs(dTargetAngle - dCurAngle);
        int msec = 20; //runTime / nTimes; //windows时间片最小20ms
        double step = delta / (runTime / msec);
        QTimer *pTimer = nullptr;
        {
            QMutexLocker locker(&servoTimerMutex);
            if(servoTimer.contains(nId)) {
                servoTimer[nId]->stop();
                servoTimer[nId]->deleteLater();
                servoTimer.remove(nId);
            }
            pTimer = new QTimer(this);
            servoTimer.insert(nId,pTimer);
        }
        pTimer->setInterval(msec);
        pTimer->setTimerType(Qt::PreciseTimer);
        connect(pTimer, &QTimer::timeout, this, [=]()mutable{
            double dSetAngle = 0.f;
            if ( dTargetAngle >= dCurAngle ) {
                dSetAngle = dCurAngle + step;
                if ( dSetAngle > dTargetAngle ) {
                    dSetAngle = dTargetAngle;
                }
            } else {
                dSetAngle = dCurAngle - step;
                if ( dSetAngle < dTargetAngle ) {
                    dSetAngle = dTargetAngle;
                }
            }
            m_fbx->setServoAngle(sName.c_str(), dSetAngle, dir, bSelectNode);
            if ( qFuzzyCompare(dSetAngle, dTargetAngle) ) {
                //                emit sigServoAngleChange(nId, dTargetAngle);
                {
                    QMutexLocker locker(&servoTimerMutex);
                    pTimer->stop ();
                    delete pTimer;
                    pTimer = nullptr;
                    servoTimer.remove(nId);
                }
                return ;
            }
            dCurAngle = dSetAngle;
        });
        pTimer->start();
        i++;
    }
}

void QRobotView::setServoAngle (int nId, double dTargetAngle)
{
    double servoAngle = 0.0;
    if(getServoAngle(nId,servoAngle)) {
        if(!qFuzzyCompare(dTargetAngle,servoAngle)) {
            setServoAngle(nId, dTargetAngle, true);
        }
    }
}

void QRobotView::setServoAngle (int nId, double dTargetAngle, bool bSelectNode)
{
    /**< 判定给定的舵机ID是否有效 */
    Servo* pServo = RobotConfig->queryServo(nId);
    if ( pServo == nullptr ) {
        return ;
    }

    /**< 判定给定的目标角度是否有效,并调整相对应的目标角度 */
    if ( dTargetAngle < pServo->getMinAngle() ) {
        dTargetAngle = pServo->getMinAngle();
    } else if ( dTargetAngle > pServo->getMaxAngle() ) {
        dTargetAngle = pServo->getMaxAngle();
    }

    /**< 界面角度转化模型角度 */
    uiToModelAngle(dTargetAngle, pServo->getOffsetAngle());

    QString strModelName = pServo->getModelName();
    QStringList modelNameList;
    modelNameList = strModelName.split(",",QString::SplitBehavior::SkipEmptyParts);
    // qDebug() << "QRobotView::setServoAngle" << modelNameList << modelNameList.size();
    if(modelNameList.size() == 0) {
        return;
    }
    dTargetAngle /= modelNameList.size();
    int i = 1;
    for(QString modelName : modelNameList) {
        std::string sName = modelName.toStdString();
        const char* name = sName.c_str();
        Fbx::AxisType dir = static_cast<Fbx::AxisType>(pServo->getRoateAxis());
        /**< 获取舵机当前角度 */
        double dCurAngle = m_fbx->getServoAngle(name, dir);
        if ( qFuzzyCompare(dCurAngle, dTargetAngle) ) {
            return ;
        }

        /**< 舵机角度方向调整 */
        dTargetAngle *= pServo->getRoateDir();
        if(i == 2) {
            dTargetAngle = -dTargetAngle;
        }
        m_fbx->setServoAngle(sName.c_str(), dTargetAngle, dir, bSelectNode);

        i++;
    }

    return ;
}

void QRobotView::setServoAngle(QMap<int, QPair<double, int>> servoInfo)
{
    if(!m_fbx) {
        return;
    }
    auto it = servoInfo.begin();
    for ( ; it != servoInfo.end(); it ++ ) {
        int nServoId = it.key();
        QPair<double, int> pair = it.value();
        double dAngle = pair.first;
        setServoAngle(nServoId, dAngle, true);
    }
}

bool QRobotView::getServoAngle (int nId, double &dAngle)
{
    if ( m_fbx == nullptr ) {
        return false;
    }

    /**< 判定给定的舵机ID是否有效 */
    Servo* pServo = RobotConfig->queryServo(nId);
    if ( pServo == nullptr ) {
        return false;
    }

    QString strModelName = pServo->getModelName();
    std::string sName = strModelName.toStdString();
    const char* name = sName.c_str();
    Fbx::AxisType dir = static_cast<Fbx::AxisType>(pServo->getRoateAxis());
    dAngle = m_fbx->getServoAngle(name, dir);
    /**< 模型角度转化界面角度 */
    modelToUiAngle(dAngle, pServo->getOffsetAngle());
    return true;
}

bool QRobotView::getServosAngle(QMap<int,double> &servos)
{
    const QList<int> servoIndexs = RobotConfig->getServos().keys();
    for(auto cit = servoIndexs.cbegin(); cit != servoIndexs.cend(); cit++){
        int servoId = *cit;
        double dAngle = 0.f;
        if ( !getServoAngle(servoId, dAngle) ) {
            return false;
        }
        servos.insert(servoId, dAngle);
    }
    return true;
}

void QRobotView::resetAngle ()
{
    auto mapBody = RobotConfig->getBodys();
    auto it = mapBody.begin();
    for ( ; it != mapBody.end(); it ++ ) {
        Body* pBody = it.value();
        auto mapServo = pBody->getServos();
        auto iter = mapServo.begin();
        for ( ; iter != mapServo.end(); iter ++ ) {
            Servo* pServo = iter.value();
            int nServoId = iter.key();
            setServoAngle(nServoId, pServo->getInitAngle(), false);
        }
    }
}

void QRobotView::switchWatch (CameraPerspective dir)
{
    switch (dir) {
    case CAMERA_FRONT:
        m_fbx->SetCameraPerspective(Fbx::CAMERA_FRONT);
        break;
    case CAMERA_BACK:
        m_fbx->SetCameraPerspective(Fbx::CAMERA_BACK);
        break;
    case CAMERA_TOP:
        m_fbx->SetCameraPerspective(Fbx::CAMERA_TOP);
        break;
    case CAMERA_BOTTOM:
        m_fbx->SetCameraPerspective(Fbx::CAMERA_BOTTOM);
        break;
    case CAMERA_LEFT:
        m_fbx->SetCameraPerspective(Fbx::CAMERA_LEFT);
        break;
    case CAMERA_RIGHT:
        m_fbx->SetCameraPerspective(Fbx::CAMERA_RIGHT);
        break;
    case CAMERA_RIGHT_45:
        m_fbx->SetCameraPerspective(Fbx::CAMERA_RIGHT_45);
        break;
    default:
        break;
    }
}

void QRobotView::switchStance (EM_STANCE_TYPE type)
{
    const QList<int> servoIndexs = RobotConfig->getServos().keys();
    for(auto cit = servoIndexs.cbegin(); cit != servoIndexs.cend(); cit++){
        int servoId = *cit;
        setServoAngle(servoId, 0.f, 1000, false);
    }
    switch (type) {
    case EM_STANCE_STAND:
        break;
    case EM_STANCE_SQUAT:
        // 待定
        break;
    case EM_STANCE_SPREAD_ARMS:
        //        setServoAngle(static_cast<int>(Robotcfg::EM_SERVO_4), 90.0f, 1000, false);
        //        setServoAngle(static_cast<int>(Robotcfg::EM_SERVO_9), -90.0f, 1000, false);
        break;
    default:
        break;
    }
}

void QRobotView::registCallBack ()
{
    m_fbx->setServoAngleChangeCallBack(
                std::bind(&QRobotView::onServoAngleChange,
                          this,
                          std::placeholders::_1,
                          std::placeholders::_2)
                );

    m_fbx->getServoAxisNameCallBack(
                std::bind(&QRobotView::getServoAxisName,
                          this,
                          std::placeholders::_1)
                );
    m_fbx->setMouseMoveAxisNameCallBack(
                std::bind(&QRobotView::getMouseMoveServoAxisName,
                          this,
                          std::placeholders::_1)
                );

    m_fbx->getServoRotationRangeCallBack(
                std::bind(&QRobotView::getServoRotationRange,
                          this,
                          std::placeholders::_1,
                          std::placeholders::_2,
                          std::placeholders::_3)
                );
}

void QRobotView::onServoAngleChange (const char* nodeName, double dAngle)
{
    int nServoId = 0;
    auto mapBody = RobotConfig->getBodys();
    auto it = mapBody.begin();
    for ( ; it != mapBody.end(); it ++ ) {
        Body* pBody = it.value();
        auto mapServo = pBody->getServos();
        auto iter = mapServo.begin();
        for ( ; iter != mapServo.end(); iter ++ ) {
            Servo* pServo = iter.value();
            if ( pServo->getModelName().compare(nodeName, Qt::CaseSensitive) == 0 ) {
                modelToUiAngle(dAngle, pServo->getOffsetAngle());
                dAngle *= pServo->getRoateDir();
                nServoId = iter.key();
                break;
            }
        }
    }

    if ( nServoId != 0 ) {
        emit sigServoAngleChange(nServoId, dAngle);
    }
}

std::string QRobotView::getServoAxisName (const char* nodeName)
{
    auto mapBody = RobotConfig->getBodys();
    auto it = mapBody.begin();
    for ( ; it != mapBody.end(); it ++ ) {
        int nBodyId = it.key();
        Body* pBody = it.value();
        auto mapServo = pBody->getServos();
        auto iter = mapServo.begin();
        for ( ; iter != mapServo.end(); iter ++ ) {
            Servo* pServo = iter.value();
            if ( pServo->getModelName().compare(nodeName, Qt::CaseSensitive) == 0 ) {
                Fbx::AxisType dir = static_cast<Fbx::AxisType>(pServo->getRoateAxis());
                int nServoId = iter.key();
                emit sigCurrentSelectServoId(nServoId, nBodyId);
                return m_fbx->getAxisName(dir);
            }
        }
    }

    return "";
}

std::string QRobotView::getMouseMoveServoAxisName(const char *nodeName)
{
    auto mapBody = RobotConfig->getBodys();
    auto it = mapBody.begin();
    for ( ; it != mapBody.end(); it ++ ) {
        int nBodyId = it.key();
        Body* pBody = it.value();
        auto mapServo = pBody->getServos();
        auto iter = mapServo.begin();
        for ( ; iter != mapServo.end(); iter ++ ) {
            Servo* pServo = iter.value();
            if ( pServo->getModelName().compare(nodeName, Qt::CaseSensitive) == 0 ) {
                Fbx::AxisType dir = static_cast<Fbx::AxisType>(pServo->getRoateAxis());
                int nServoId = iter.key();
                showMenuWithServoId(nBodyId,nServoId);
                return m_fbx->getAxisName(dir);
            }
        }
    }

    return "";
}

bool QRobotView::getServoRotationRange (const char* nodeName, double& dMinAngle, double& dMaxAngle)
{
    auto mapBody = RobotConfig->getBodys();
    auto it = mapBody.begin();
    for ( ; it != mapBody.end(); it ++ ) {
        Body* pBody = it.value();
        auto mapServo = pBody->getServos();
        auto iter = mapServo.begin();
        for ( ; iter != mapServo.end(); iter ++ ) {
            Servo* pServo = iter.value();
            if ( pServo->getModelName().compare(nodeName, Qt::CaseSensitive) == 0 ) {
                dMaxAngle = pServo->getMaxAngle();
                dMinAngle = pServo->getMinAngle();
                uiToModelAngle(dMaxAngle, pServo->getOffsetAngle());
                uiToModelAngle(dMinAngle, pServo->getOffsetAngle());
                int dir = pServo->getRoateDir();
                dMaxAngle *= dir;
                dMinAngle *= dir;
                if ( dMaxAngle < dMinAngle ) {
                    double temp = dMaxAngle;
                    dMaxAngle = dMinAngle;
                    dMinAngle = temp;
                }
                return true;
            }
        }
    }
    return false;
}

void QRobotView::setSelectedNode(int nodeId)
{
    Servo* pServo = RobotConfig->queryServo(nodeId);
    QString strModelName = pServo->getModelName();
    std::string sName = strModelName.toStdString();
    const char* name = sName.c_str();
    m_fbx->setSelectedNode(name);
}

void QRobotView::initializeGL()
{
    initializeOpenGLFunctions();
    if ( m_fbx == nullptr ) {
        QSurfaceFormat fmt;
        fmt.setSamples(16);
        fmt.setDepthBufferSize(24);
        fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        this->context()->setFormat(fmt);

        m_fbx = new Fbx(MODEL_PATH, "");
        registCallBack();
    }
}

void QRobotView::resizeGL(int w, int h)
{
    m_fbx->OnReshape(w, h);
}

void QRobotView::paintGL()
{
    m_fbx->OnDisplay();
    if ( m_fbx->GetStatus() == Fbx::MUST_BE_LOADED ) {
        m_fbx->LoadFile();
        resetAngle();
        m_fbx->SetAnimStackSelection();
        QTimer* pTimer = new QTimer(this);
        pTimer->setInterval((int)m_fbx->GetFrameMillTime());
        pTimer->setTimerType(Qt::PreciseTimer);
        connect(pTimer, &QTimer::timeout, this, [=](){
            if (m_fbx->GetStatus() == Fbx::MUST_BE_REFRESHED) {
                update();
            }
            m_fbx->OnTimerClick();
        });
        pTimer->start();
    }
    static bool firstPaint = true;
    if(firstPaint) {
        firstPaint = false;
        if(RobotConfig->getRobotType() == Robotcfg::EM_ROBOT_YANSHEE15
                || RobotConfig->getRobotType() == Robotcfg::EM_ROBOT_YANSHEE11) {
            m_fbx->SetCameraPerspective(Fbx::CAMERA_BACK);
        }
    }
}


void QRobotView::mousePressEvent(QMouseEvent *event)
{

    const char * selectorNode  = m_fbx->getSelectorFbxNode();
    if(selectorNode != NULL){
        qDebug() << "selectorNode====" << selectorNode;
        if(strcmp(selectorNode,"body_02") != 0){
            m_isSelectNode = true;
            getServoIdForNodeName(selectorNode,&mSelectServoId,&mSelectBodyId);
            DataCenter::getInstance()->sendAdjustServo(mSelectServoId);
            mCurrentAngle = DataCenter::getInstance()->getAngleForServoId(mSelectServoId);
            qDebug() << "servoId=111==" << mSelectServoId << ";;bodyId==111==" << mSelectBodyId;
            showMouseCursor(mSelectServoId);
        }


    }else {
        qDebug() << "selectorNode==is NULL==";
        m_isSelectNode = false;

    }
    m_bMouseMove = false;
    m_isPressed = true;
    mouseEvent(event);
}

void QRobotView::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mouseReleaseEvent=======";
    mouseEvent(event);
    m_bMouseMove = false;
    m_isPressed = false;
    if(m_isSelectNode){
        QPoint pt = event->pos();
        pressPoint = pt;
        makeCurrent();
        m_fbx->OnMouseMotion(pt.x(), pt.y());
        m_fbx->onMouseMove(pt.x(),pt.y());
        doneCurrent();
        this->setCursor(Qt::ArrowCursor);
        DataCenter::getInstance()->servoAngleChangeFinished(mSelectServoId,mCurrentAngle);
        m_isSelectNode  = false;

    }


}

void QRobotView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pt = event->pos();
    mouseMovePoint = pt;
    if(m_isSelectNode){
        moveSelectServo();
    }else {
        makeCurrent();
        if(m_isPressed){
            m_bMouseMove = true;
            m_fbx->OnMouseMotion(pt.x(), pt.y());
        }else {
            m_fbx->OnMouseMotion(pt.x(), pt.y());
            m_fbx->onMouseMove(pt.x(),pt.y());
        }
        doneCurrent();
    }


}

void QRobotView::wheelEvent(QWheelEvent *event)
{
    Fbx::WheelDir dir = Fbx::WHEEL_UP;
    QPoint pt = event->angleDelta();
    if ( pt.y() > 0 ) {
        dir = Fbx::WHEEL_DOWN;
    } else {
        dir = Fbx::WHEEL_UP;
    }
    m_fbx->OnMouseWheel(dir);
}

bool QRobotView::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == mMenu){
        if(event->type()==QEvent::MouseButtonPress){
            QMouseEvent *mouseEvent = (QMouseEvent *)event;

            if(mouseEvent->buttons()&Qt::LeftButton){
                qDebug() << "event==MouseButtonPress====" << event->type();

                QPoint mousePos = mouseEvent->globalPos();
                QPoint menuPos =  mapToGlobal(mouseMovePoint);


                QRect rect(menuPos.x(),menuPos.y(),mMenu->width(),mMenu->height());
                qDebug() << "mousePos===" << mousePos << ";rect===" << rect;
                if(rect.contains(mousePos)){
                    pressPoint = mouseEvent->pos();
                    m_isMenuSelectNode = true;
                    DataCenter::getInstance()->sendAdjustServo(mSelectServoId);
                    mMenu->setWindowOpacity(0);
                    showMouseCursor(mSelectServoId);
                    mCurrentAngle = DataCenter::getInstance()->getAngleForServoId(mSelectServoId);
                    return true;
                }
            }

        }

        if(event->type()==QEvent::MouseMove){

            QMouseEvent *mouseEvent = (QMouseEvent *)event;
            if(mouseEvent->buttons()&Qt::LeftButton && m_isMenuSelectNode){
                qDebug() << "event==MouseMove====" << event->type();
                mouseMovePoint = mouseEvent->pos();
                moveSelectServo();
            }

        }
        if(event->type()==QEvent::MouseButtonRelease){
            if(m_isMenuSelectNode){
                qDebug() << "event==MouseButtonRelease====" << event->type();
                mMenu->setCursor(Qt::ArrowCursor);
                mMenu->close();
                mMenu = nullptr;
                DataCenter::getInstance()->servoAngleChangeFinished(mSelectServoId,mCurrentAngle);
                m_isMenuSelectNode = false;
            }
        }


        if(event->type() == QEvent::MouseMove && !m_isMenuSelectNode){
            auto pos = mapToGlobal(mMenu->pos());
            QRect rect(pos.x()-200,pos.y()-50,mMenu->width()+50,mMenu->height()+100);
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            QPoint point = mouseEvent->globalPos();
            int x = point.x()+200;
            int y = point.y() + 100;
            QPoint p(x,y);
            if(!rect.contains(p)){
                mMenu->close();
                mMenu = nullptr;

            }
        }

    }

    return false;
}

void QRobotView::mouseEvent (QMouseEvent *event)
{
    Fbx::Button button = Fbx::LEFT_BUTTON;
    Qt::MouseButton qBtn = event->button();
    switch (qBtn) {
    case Qt::RightButton:
        button = Fbx::RIGHT_BUTTON;
        break;
    case Qt::LeftButton:
        button = Fbx::LEFT_BUTTON;
        break;
    case Qt::MiddleButton:
        button = Fbx::MIDDLE_BUTTON;
    default:
        break;
    }

    Fbx::State state = Fbx::BUTTON_DOWN;
    QEvent::Type type = event->type();
    switch(type) {
    case QEvent::MouseButtonPress:
        state = Fbx::BUTTON_DOWN;
        break;
    case QEvent::MouseButtonRelease:
        state = Fbx::BUTTON_UP;
        break;
    }

    QPoint pos = event->pos();
    //    if ( qBtn == Qt::RightButton && type == QEvent::MouseButtonRelease && !m_bMouseMove){
    // 弹出菜单
    //        QMenu menu(this);
    //        menu.addAction(QStringLiteral("Front"), this,
    //                       [=](){
    //            if(RobotConfig->getRobotType() == Robotcfg::EM_ROBOT_YANSHEE15
    //                    || RobotConfig->getRobotType() == Robotcfg::EM_ROBOT_YANSHEE11) {
    //                m_fbx->SetCameraPerspective(Fbx::CAMERA_BACK);
    //            } else {
    //                m_fbx->SetCameraPerspective(Fbx::CAMERA_FRONT);
    //            }

    //        } );
    //        menu.addAction(QStringLiteral("Back"), this,
    //                       [=](){
    //            if(RobotConfig->getRobotType() == Robotcfg::EM_ROBOT_YANSHEE15
    //                    || RobotConfig->getRobotType() == Robotcfg::EM_ROBOT_YANSHEE11) {
    //                m_fbx->SetCameraPerspective(Fbx::CAMERA_FRONT);
    //            } else {
    //                m_fbx->SetCameraPerspective(Fbx::CAMERA_BACK);
    //            }
    //        } );
    //        menu.addAction(QStringLiteral("Top"), this,
    //                       [=](){
    //            m_fbx->SetCameraPerspective(Fbx::CAMERA_TOP);
    //        } );
    //        menu.addAction(QStringLiteral("Bottom"), this,
    //                       [=](){
    //            m_fbx->SetCameraPerspective(Fbx::CAMERA_BOTTOM);
    //        } );
    //        menu.addAction(QStringLiteral("Left"), this,
    //                       [=](){
    //            m_fbx->SetCameraPerspective(Fbx::CAMERA_LEFT);
    //        } );
    //        menu.addAction(QStringLiteral("Right"), this,
    //                       [=](){
    //            m_fbx->SetCameraPerspective(Fbx::CAMERA_RIGHT);
    //        } );
    //        menu.addAction(QStringLiteral("Right_45"), this,
    //                       [=](){
    //            m_fbx->SetCameraPerspective(Fbx::CAMERA_RIGHT_45);
    //        });
    //        menu.exec(mapToGlobal(pos));
    //    }

    /**
     * @brief 鼠标点击事件必须确保在上下文中绑定，否则无法获取三维深度。
     */
    makeCurrent();
    m_fbx->OnMouse(button, state, pos.x(), pos.y(), m_bMouseMove);
    doneCurrent();
}

double QRobotView::uiToModelAngle (double& dUiAngle, double dOffset)
{
    /**< 界面角度 + offset = 模型角度 */
    dUiAngle += dOffset;
    return dUiAngle;
}

double QRobotView::modelToUiAngle (double& dModelAngle, double dOffset)
{
    /**< 界面角度 + offset = 模型角度 */
    dModelAngle -= dOffset;
    return dModelAngle;
}

void QRobotView::showMenuWithServoId(int bodyId, int servoId)
{

    int firstServoId;
    int secondServoId;
    if(servoId == 2){
        firstServoId = 1;
        secondServoId = 2;

    }else if(servoId == 5){
        firstServoId = 4;
        secondServoId = 5;
    }else {
        emit sigCurrentSelectServoId(servoId, bodyId);
        return;
    }
    if(mMenu == nullptr){
        mMenu = new QMenu(this);
        mMenu->setWindowFlags(mMenu->windowFlags() | Qt::NoDropShadowWindowHint);
        mMenu->installEventFilter(this);
        mMenu->setMouseTracking(true);
        mMenu->setAttribute(Qt::WA_DeleteOnClose);
        connect(mMenu,&QMenu::destroyed,this,[=](){
           if(mMenu != nullptr){
               mMenu = nullptr;
           }
        });
        Servo* firstServo = RobotConfig->getServoFromId(firstServoId);
        QAction *action1 = mMenu->addAction(firstServo->getName());
        connect(action1,&QAction::hovered,this,[=](){
            if(!m_isMenuSelectNode){
                mSelectServoId = firstServoId;
                mSelectBodyId = bodyId;
                mSelectServo = firstServo;
                setSelectedNode(mSelectServoId);
                emit sigCurrentSelectServoId(firstServoId, bodyId);
            }
        });

        Servo* secondServo = RobotConfig->getServoFromId(secondServoId);
        QAction *action2 = mMenu->addAction(secondServo->getName());

        connect(action2,&QAction::hovered,this,
                [=](){
            if(!m_isMenuSelectNode){
                mSelectServoId = secondServoId;
                mSelectBodyId = bodyId;
                mSelectServo = secondServo;
                setSelectedNode(mSelectServoId);
                emit sigCurrentSelectServoId(secondServoId, bodyId);
            }
        });

        //        mMenu->exec(mapToGlobal(mouseMovePoint));
        mMenu->popup(mapToGlobal(mouseMovePoint));
    }
}


void QRobotView::showMouseCursor(int serovId)
{
    if(serovId == 17 || serovId == 2
            || serovId == 3 || serovId == 5 || serovId == 6
            || serovId == 7 || serovId == 11 || serovId == 12 || serovId == 16){
        const QCursor myCursor(QPixmap(":/image/res/image/ic_cross.png"),-1,-1);
        if(m_isMenuSelectNode){
            if(mMenu != nullptr && m_isMenuSelectNode){
                mMenu->setCursor(myCursor);
            }
        }else {

            this->setCursor(myCursor);
        }
    }else if(serovId == 1 || serovId == 4
             || serovId == 8 || serovId == 9 || serovId == 10
             || serovId == 13 || serovId == 14 || serovId == 15 ){
        const QCursor myCursor(QPixmap(":/image/res/image/ic_vertical.png"),-1,-1);

        if(m_isMenuSelectNode){
            if(mMenu != nullptr && m_isMenuSelectNode){
                mMenu->setCursor(myCursor);
            }
        }else {

            this->setCursor(myCursor);
        }

    }
}

void QRobotView::getServoIdForNodeName(const char *nodeNome, int *servoId, int *bodyId)
{
    auto mapBody = RobotConfig->getBodys();
    auto it = mapBody.begin();
    for ( ; it != mapBody.end(); it ++ ) {
        *bodyId = it.key();
        Body* pBody = it.value();
        auto mapServo = pBody->getServos();
        auto iter = mapServo.begin();
        for ( ; iter != mapServo.end(); iter ++ ) {
            Servo* pServo = iter.value();
            if ( pServo->getModelName().compare(nodeNome, Qt::CaseSensitive) == 0 ) {
                *servoId = iter.key();
                mSelectServo = pServo;
                return;
            }
        }
    }
}

void QRobotView::moveSelectServo()
{
    if(mSelectServoId == 17){ //左右移动的舵机
        servoMoveToX(true);
    }else if(mSelectServoId == 5 || mSelectServoId == 2
             || mSelectServoId == 11 || mSelectServoId == 16
             || mSelectServoId == 7||mSelectServoId == 12
             || mSelectServoId == 6 || mSelectServoId == 3){
        servoMoveToX(false);
    }else if(mSelectServoId == 14 || mSelectServoId == 8
             ||mSelectServoId == 1|| mSelectServoId == 15){
        servoMoveToY(false);
    }else if( mSelectServoId == 4
             || mSelectServoId == 8 || mSelectServoId == 9
             || mSelectServoId == 10|| mSelectServoId == 13
             || mSelectServoId == 14){ //上下移动的舵机
         servoMoveToY(true);
    }


}

void QRobotView::servoMoveToX(bool isLeft)
{
    int pressX = pressPoint.x();
    int moveX = mouseMovePoint.x();
    int dx = moveX - pressX;
    if(qAbs(moveX - pressX) >=1){
        pressPoint = mouseMovePoint;
        if(dx > 0){
            if(isLeft){
                mCurrentAngle = mCurrentAngle - 2;
            }else {
                mCurrentAngle = mCurrentAngle + 2;
            }
        }else {
            if(isLeft){
                mCurrentAngle = mCurrentAngle + 2;
            }else {
                mCurrentAngle = mCurrentAngle - 2;
            }
        }

        if(mCurrentAngle>=mSelectServo->getMaxAngle()){
            mCurrentAngle = mSelectServo->getMaxAngle();
        }
        if(mCurrentAngle <= mSelectServo->getMinAngle()){
            mCurrentAngle = mSelectServo->getMinAngle();
        }
        if(DataCenter::getInstance()->servoMirrorEnabled(mSelectServoId)){
            int oppositeServoId = mSelectServoId;
            double oppositeServoAngle = mCurrentAngle;
            RobotConfig->calcOppositeServoValue(oppositeServoId,oppositeServoAngle);

            DataCenter::getInstance()->changeCurrentFrameServoNotToRobot(mSelectServoId,mCurrentAngle);
            DataCenter::getInstance()->changeCurrentFrameServoNotToRobot(oppositeServoId,oppositeServoAngle);


        }else {
            DataCenter::getInstance()->changeCurrentFrameServoNotToRobot(mSelectServoId,mCurrentAngle);
        }

        DataCenter::getInstance()->emitCurrentServosData();
    }
}

void QRobotView::servoMoveToY(bool isUp)
{
    int pressY = pressPoint.y();
    int moveY = mouseMovePoint.y();
    int dy = moveY - pressY;
    if(qAbs(moveY - pressY) >=1){
        pressPoint = mouseMovePoint;
        if(dy > 0){
            if(isUp){
                mCurrentAngle = mCurrentAngle - 2;
            }else {
                mCurrentAngle = mCurrentAngle + 2;
            }
        }else {
            if(isUp){
                mCurrentAngle = mCurrentAngle + 2;
            }else {
                mCurrentAngle = mCurrentAngle - 2;
            }
        }

        if(mCurrentAngle>=mSelectServo->getMaxAngle()){
            mCurrentAngle = mSelectServo->getMaxAngle();
        }
        if(mCurrentAngle <= mSelectServo->getMinAngle()){
            mCurrentAngle = mSelectServo->getMinAngle();
        }

        if(DataCenter::getInstance()->servoMirrorEnabled(mSelectServoId)){
            int oppositeServoId = mSelectServoId;
            double oppositeServoAngle = mCurrentAngle;
            RobotConfig->calcOppositeServoValue(oppositeServoId,oppositeServoAngle);

            DataCenter::getInstance()->changeCurrentFrameServoNotToRobot(mSelectServoId,mCurrentAngle);
            DataCenter::getInstance()->changeCurrentFrameServoNotToRobot(oppositeServoId,oppositeServoAngle);


        }else {
            DataCenter::getInstance()->changeCurrentFrameServoNotToRobot(mSelectServoId,mCurrentAngle);
        }

        DataCenter::getInstance()->emitCurrentServosData();
    }
}

void QRobotView::stopSetAngle()
{
    QMutexLocker locker(&servoTimerMutex);
    for(auto cit = servoTimer.cbegin(); cit != servoTimer.cend(); ++cit) {
        cit.value()->stop();
        cit.value()->deleteLater();
    }
    servoTimer.clear();
}

