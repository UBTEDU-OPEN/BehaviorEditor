#include "jointadjuestmentwidget.h"
#include "ui_jointadjuestmentwidget.h"
#include<QHBoxLayout>
#include<QFile>
#include<QToolButton>
#include<QDebug>
#include<QAction>
#include<QStyle>
#include<robotcfg.h>
#include"servoform.h"
#include<QMap>
#include"datacenter.h"
#include<QThread>
#include<QTimer>

JointAdjuestmentWidget::JointAdjuestmentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JointAdjuestmentWidget),
    mCurrJointId(0),
    mHandMirrorEnabled(false),
    mLegMirrorEnabled(false)

{
    ui->setupUi(this); 
    ui->mirrorBtn->setProperty("isMirrorOpen",false);
    connect(ui->mirrorBtn,&QToolButton::clicked,this,[=](){
        reverseCurrentLimbMirrorEnabled();
    });
    setMirrorBtnVisible(false);
    connect(ui->jointAdjWidget,&JointSelectorWidget::jointSelected,this,&JointAdjuestmentWidget::onJointSelected);
    connect(ui->jointAdjWidget,&JointSelectorWidget::jointLocked,this,&JointAdjuestmentWidget::onJointLocked);
    connect(DataCenter::getInstance(),&DataCenter::playStateChanged,this,&JointAdjuestmentWidget::onPlayStateChange);


    createServoUI();
}

JointAdjuestmentWidget::~JointAdjuestmentWidget()
{
    delete ui;
}

void JointAdjuestmentWidget::updateServosInfo(int joint)
{
    auto pBody = RobotConfig->queryBody(joint);
    const auto servos = pBody->getServos();
    for(ServoForm* servoForm:mServoWidgets){
        servoForm->setVisible(false);
    }
    for(ServoForm* servoForm:mServoWidgets){
        QString formName = servoForm->objectName();
        for(auto cit = servos.cbegin();cit != servos.end(); cit++){
            int serovId = cit.key();
            Servo* servo = cit.value();
            if(formName == servo->getModelName()){
                servoForm->setValue(mServosValue[serovId]);
                servoForm->setLockState(mServosLockFlag[serovId]);
                servoForm->setVisible(true);
            }
        }
    }

}

void JointAdjuestmentWidget::reverseCurrentLimbMirrorEnabled()
{
    auto limbType = RobotConfig->queryBody(mCurrJointId)->getType();
    if(limbType == Robotcfg::EM_BODY_RIGHT_HAND || limbType == Robotcfg::EM_BODY_LEFT_HAND) {
        mHandMirrorEnabled = !mHandMirrorEnabled;
        ui->mirrorBtn->style()->unpolish(ui->mirrorBtn);
        ui->mirrorBtn->setProperty("isMirrorOpen",mHandMirrorEnabled);
        ui->mirrorBtn->style()->polish(ui->mirrorBtn);
        emit handsMirrorStateChanged(mHandMirrorEnabled);
        if(!ui->jointAdjWidget->getIsLockedForJoint(2) && !ui->jointAdjWidget->getIsLockedForJoint(3)){
            ui->jointAdjWidget->setAllServorsUnlocked(false,2);
            ui->jointAdjWidget->setAllServorsUnlocked(false,3);
            lockedForJointId(2,true);
            lockedForJointId(3,true);
            DataCenter::getInstance()->onChangeLimbLockState(2,true);
            DataCenter::getInstance()->onChangeLimbLockState(3,true);
        }else {
            setServorForMirrorBtn(true);
        }
    } else if(limbType == Robotcfg::EM_BODY_RIGHT_LEG || limbType == Robotcfg::EM_BODY_LEFT_LEG) {
        mLegMirrorEnabled = !mLegMirrorEnabled;
        ui->mirrorBtn->style()->unpolish(ui->mirrorBtn);
        ui->mirrorBtn->setProperty("isMirrorOpen",mLegMirrorEnabled);
        ui->mirrorBtn->style()->polish(ui->mirrorBtn);
        emit legsMirrorStateChanged(mLegMirrorEnabled);
        if(!ui->jointAdjWidget->getIsLockedForJoint(4) && !ui->jointAdjWidget->getIsLockedForJoint(5)){
            ui->jointAdjWidget->setAllServorsUnlocked(false,4);
            ui->jointAdjWidget->setAllServorsUnlocked(false,5);
            lockedForJointId(4,true);
            lockedForJointId(5,true);
            DataCenter::getInstance()->onChangeLimbLockState(4,true);
            DataCenter::getInstance()->onChangeLimbLockState(5,true);
        }else {
            setServorForMirrorBtn(false);
        }
    }
}

void JointAdjuestmentWidget::createServoUI()
{
    const auto& limbs = RobotConfig->getBodys();
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0,10,0,0);
    vLayout->setSpacing(10);
    int index = 0;
    for(auto cit = limbs.cbegin(); cit != limbs.cend(); cit++) {
        int limbId = cit.key();
        Body* limb = cit.value();
        ui->jointAdjWidget->setJointName(limbId,limb->getName());
        const auto& servos = limb->getServos();
        int servoCount = servos.size();
        for(auto sit = servos.cbegin(); sit != servos.cend(); sit++){
            int sKey = sit.key();
            Servo* sValue = sit.value();

            ServoForm* servoForm = new ServoForm(this);
            servoForm->setMinimumHeight(72);
            servoForm->setMaximumHeight(72);
            servoForm->setObjectName(sValue->getModelName());
            servoForm->setId(sKey);
            servoForm->setName(sValue->getName());
            servoForm->setMaxValue(sValue->getMaxAngle());
            servoForm->setMinValue(sValue->getMinAngle());
            servoForm->setBodyId(limbId);
            connect(servoForm,&ServoForm::servoAngleChanged,this,&JointAdjuestmentWidget::onServoValueChanged);
            connect(servoForm,&ServoForm::servoAngleChangeFinished,this,&JointAdjuestmentWidget::servoAngleChangeFinished);
            connect(servoForm,&ServoForm::servoLockStateChanged,this,&JointAdjuestmentWidget::onServoLockStateChanged);
            connect(servoForm,&ServoForm::sigServoSelect,this,&JointAdjuestmentWidget::onServoSelect);
            vLayout->addWidget(servoForm);
            ui->widget->setLayout(vLayout);
            mServoWidgets.push_back(servoForm);
            servoForm->setVisible(false);

            mServosValue.insert(sKey,sValue->getInitAngle());
            mServosLockFlag.insert(sKey,true);

            index++;
        }
    }
}

void JointAdjuestmentWidget::setMirrorBtnVisible(bool visible)
{
    if(visible) {
        switch (mCurrJointId) {
        case 2:
        case 3:
            ui->mirrorBtn->style()->unpolish(ui->mirrorBtn);
            ui->mirrorBtn->setProperty("isMirrorOpen",mHandMirrorEnabled);
            ui->mirrorBtn->style()->polish(ui->mirrorBtn);
            break;
        case 4:
        case 5:
            ui->mirrorBtn->style()->unpolish(ui->mirrorBtn);
            ui->mirrorBtn->setProperty("isMirrorOpen",mLegMirrorEnabled);
            ui->mirrorBtn->style()->polish(ui->mirrorBtn);
            break;
        default:
            break;
        }
    }

    ui->mirrorBtn->setVisible(visible);
    ui->mirroLabel->setVisible(visible);

}

void JointAdjuestmentWidget::servoSelect(int servoId)
{
    foreach (ServoForm *from, mServoWidgets) {
        int id  = from->getServoId();
        if(servoId == id){
            from->setSelected(true);
        }else {
            from->setSelected(false);
        }
    }
}

void JointAdjuestmentWidget::lockedForJointId(int joint, bool isLocked)
{
    auto pBody = RobotConfig->queryBody(joint);
    const auto servos = pBody->getServos();


    for(ServoForm* servoForm:mServoWidgets){
        int id = servoForm->getServoId();
        for (auto sit = servos.cbegin(); sit != servos.cend(); sit++) {
            int servoId= sit.key();
            if(id == servoId) {
                mServosLockFlag[id] = isLocked;
                servoForm->setLockState(isLocked);
            }
        }
    }
}

int JointAdjuestmentWidget::getMirrorServoId(int servoId)
{
    if(servoId == 1){
        return 4;
    }else if(servoId == 2){
        return 5;
    }else if(servoId == 3){
        return 6;
    }else if(servoId == 4){
        return 1;
    }else if(servoId == 5){
        return 2;
    }else if(servoId == 6){
        return 3;
    }else if(servoId == 7){
        return 12;
    }else if(servoId == 8){
        return 13;
    }else if(servoId == 9){
        return 14;
    }else if(servoId == 10){
        return 15;
    }else if(servoId == 11){
        return 16;
    }else if(servoId == 12){
        return 7;
    }else if(servoId == 13){
        return 8;
    }else if(servoId == 14){
        return 9;
    }else if(servoId == 15){
        return 10;
    }else if(servoId == 16){
        return 11;
    }
}

void JointAdjuestmentWidget::setServorForMirrorBtn(bool isHand)
{
    for(auto it = mServosLockFlag.cbegin();it != mServosLockFlag.cend();it++){
        int servoId = it.key();
        int mirrorId = getMirrorServoId(servoId);
        if(isHand){
            if(servoId == 1 || servoId ==2 || servoId == 3){
                if(!mServosLockFlag[servoId] && !mServosLockFlag[mirrorId]){
                    for(ServoForm* servoForm:mServoWidgets){
                        int id = servoForm->getServoId();
                        if(id == servoId ||id == mirrorId) {
                            mServosLockFlag[id] = true;
                            servoForm->setLockState(true);
                            DataCenter::getInstance()->onChangeServoLockState(id,true);
                            ui->jointAdjWidget->setAllServorsUnlocked(false,2);
                            ui->jointAdjWidget->setAllServorsUnlocked(false,3);
                        }
                    }
                }
            }
        }else {
            if(servoId == 7 || servoId ==8 || servoId == 9 || servoId == 10 || servoId == 11){
                if(!mServosLockFlag[servoId] && !mServosLockFlag[mirrorId]){
                    for(ServoForm* servoForm:mServoWidgets){
                        int id = servoForm->getServoId();
                        if(id == servoId ||id == mirrorId) {
                            mServosLockFlag[id] = true;
                            servoForm->setLockState(true);
                            DataCenter::getInstance()->onChangeServoLockState(id,true);
                            ui->jointAdjWidget->setAllServorsUnlocked(false,4);
                            ui->jointAdjWidget->setAllServorsUnlocked(false,5);
                        }
                    }
                }
            }
        }

    }
}



void JointAdjuestmentWidget::onJointSelected(int joint, bool isSelected)
{
    qDebug() << "joint===" << joint;
    mCurrJointId = joint;
    switch (joint) {
    case 1: //头部
        setMirrorBtnVisible(false);
        updateServosInfo(joint);
        break;
    case 3://左臂
        setMirrorBtnVisible(true);
        updateServosInfo(joint);
        break;
    case 2://右臂
        setMirrorBtnVisible(true);
        updateServosInfo(joint);
        break;
    case 5://左腿
        setMirrorBtnVisible(true);
        updateServosInfo(joint);
        break;
    case 4://右腿
        setMirrorBtnVisible(true);
        updateServosInfo(joint);
        break;

    default:
        break;
    }
}

void JointAdjuestmentWidget::resetLockState()
{
    ui->jointAdjWidget->resetLockState();
}

void JointAdjuestmentWidget::resetMirrorState()
{
    mHandMirrorEnabled = false;
    mLegMirrorEnabled = false;
    ui->mirrorBtn->style()->unpolish(ui->mirrorBtn);
    ui->mirrorBtn->setProperty("isMirrorOpen",false);
    ui->mirrorBtn->style()->polish(ui->mirrorBtn);
}

void JointAdjuestmentWidget::onPostureReset(const QMap<int,double>& servos)
{
    mServosValue = servos;
//    resetLockState();
    resetMirrorState();
//    if(mCurrJointId != 0){
//        updateServosInfo(mCurrJointId);
//    }
    ui->jointAdjWidget->setAllServorsUnlocked(false,1);
    ui->jointAdjWidget->setAllServorsUnlocked(false,2);
    ui->jointAdjWidget->setAllServorsUnlocked(false,3);
    ui->jointAdjWidget->setAllServorsUnlocked(false,4);
    ui->jointAdjWidget->setAllServorsUnlocked(false,5);
    for(auto it = mServosLockFlag.cbegin();it != mServosLockFlag.cend();it++){
        int id = it.key();
        mServosLockFlag[id] = true;
    }
    for(auto it = mServoWidgets.begin(); it != mServoWidgets.end();it++){
        ServoForm *from = *it;
        from->setValue(servos[from->getServoId()]);
        from->setLockState(true);
    }
}

void JointAdjuestmentWidget::onPlayStateChange(DataCenter::PlayActionType playType)
{
    switch (playType) {
    case DataCenter::kPlayAll:
    case DataCenter::kPlayAction:
    case DataCenter::kPlayMusic:
        for(auto it = mServosLockFlag.cbegin();it != mServosLockFlag.cend();it++){
            int id = it.key();
            mServosLockFlag[id] = true;
        }
        break;
    case DataCenter::kPlayNone:

        break;
    default:
        break;
    }
}

void JointAdjuestmentWidget::onServoSelect(int servoId)
{
    if(mCurrSelectServoId != servoId){
        mCurrSelectServoId = servoId;
        servoSelect(servoId);
    }
    emit sigServoSelected(servoId);
}

void JointAdjuestmentWidget::onJointLocked(int joint, bool isLocked)
{

    lockedForJointId(joint,isLocked);
    if(!isLocked &&mHandMirrorEnabled){
        if((joint ==3 && !ui->jointAdjWidget->getIsLockedForJoint(2)) ||(joint == 2 && !ui->jointAdjWidget->getIsLockedForJoint(3))){
            mHandMirrorEnabled = false;
            ui->mirrorBtn->style()->unpolish(ui->mirrorBtn);
            ui->mirrorBtn->setProperty("isMirrorOpen",mHandMirrorEnabled);
            ui->mirrorBtn->style()->polish(ui->mirrorBtn);
            emit handsMirrorStateChanged(mHandMirrorEnabled);
        }
    }

    if(!isLocked && mLegMirrorEnabled){
        if((joint ==4 && !ui->jointAdjWidget->getIsLockedForJoint(5)) ||(joint == 5 && !ui->jointAdjWidget->getIsLockedForJoint(4))){
            mLegMirrorEnabled = false;
            ui->mirrorBtn->style()->unpolish(ui->mirrorBtn);
            ui->mirrorBtn->setProperty("isMirrorOpen",mLegMirrorEnabled);
            ui->mirrorBtn->style()->polish(ui->mirrorBtn);
            emit legsMirrorStateChanged(mLegMirrorEnabled);
        }
    }
    DataCenter::getInstance()->onChangeLimbLockState(joint,isLocked);
}

void JointAdjuestmentWidget::onServoValueChanged(int servoId, double value)
{

    mServosValue[servoId] = value;
    if(mLegMirrorEnabled || mHandMirrorEnabled){
        auto limbType = RobotConfig->queryBody(mCurrJointId)->getType();
        if(limbType != Robotcfg::EM_BODY_HEAD){
            int oppositeServoId = servoId;
            double oppositeServoAngle = value;
            RobotConfig->calcOppositeServoValue(oppositeServoId,oppositeServoAngle);
            mServosValue[oppositeServoId] = oppositeServoAngle;
            //TODO: 需要合并接口，要不然机器人可能处理不过来
            emit servoAngleChanged(servoId,value);
            emit servoAngleChanged(oppositeServoId,oppositeServoAngle);
        }else {
            emit servoAngleChanged(servoId,value);
        }
    }else {
        emit servoAngleChanged(servoId,value);
    }
    if(mCurrSelectServoId != servoId){
        mCurrSelectServoId = servoId;
        servoSelect(servoId);
    }
}

void JointAdjuestmentWidget::onFrameIdChanged(const QMap<int, double> &servos){
    if(mCurrJointId == 0){
        return;
    }

    this->mServosValue = servos;

    updateServosInfo(this->mCurrJointId);
}



void JointAdjuestmentWidget::onServoLockStateChanged(int id, bool isLocked)
{
    mServosLockFlag[id] = isLocked;
    auto pBody = RobotConfig->queryBody(mCurrJointId);
    const auto servos = pBody->getServos();
    bool isAllServosUnlocked = true;
    for(QMap<int,bool>::iterator it = mServosLockFlag.begin();it != mServosLockFlag.end();it++){
        int id = it.key();
        bool flag = it.value();
        for(QMap<int, Servo*>::ConstIterator cit = servos.cbegin(); cit != servos.end();cit++){
            int cid = cit.key();
            if(cid == id){
                if(flag){
                    isAllServosUnlocked = false;
                    break;
                }
            }
        }
    }
    if(!isLocked && id != 0){
        if(mHandMirrorEnabled){
            bool isLocked = mServosLockFlag[id];
            int mirrorId = getMirrorServoId(id);
            bool isMirrorLocked = mServosLockFlag[mirrorId];
            if(!isLocked && !isMirrorLocked){
                mHandMirrorEnabled = false;
                ui->mirrorBtn->style()->unpolish(ui->mirrorBtn);
                ui->mirrorBtn->setProperty("isMirrorOpen",mHandMirrorEnabled);
                ui->mirrorBtn->style()->polish(ui->mirrorBtn);
                emit handsMirrorStateChanged(mHandMirrorEnabled);
            }

        }
        if(mLegMirrorEnabled){
            bool isLocked = mServosLockFlag[id];
            int mirrorId = getMirrorServoId(id);
            bool isMirrorLocked = mServosLockFlag[mirrorId];
            if(!isLocked && !isMirrorLocked){
                mLegMirrorEnabled = false;
                ui->mirrorBtn->style()->unpolish(ui->mirrorBtn);
                ui->mirrorBtn->setProperty("isMirrorOpen",mLegMirrorEnabled);
                ui->mirrorBtn->style()->polish(ui->mirrorBtn);
                emit legsMirrorStateChanged(mLegMirrorEnabled);
            }
        }
    }
    ui->jointAdjWidget->setAllServorsUnlocked(isAllServosUnlocked,mCurrJointId);
    DataCenter::getInstance()->onChangeServoLockState(id,isLocked);
}

void JointAdjuestmentWidget::onSigCurrentSelectServoId(int nId, int nBody)
{
    if(nBody == 1){
        ui->jointAdjWidget->onHeadClicked();
    }else if(nBody == 3){
        ui->jointAdjWidget->onLeftArmClicked();
    }else if(nBody == 2){
        ui->jointAdjWidget->onRightArmClicked();
    }else if(nBody == 5){
        ui->jointAdjWidget->onLeftLegClicked();
    }else if(nBody == 4){
        ui->jointAdjWidget->onRightLegClicked();
    }
    mCurrSelectServoId = nId;
    servoSelect(nId);
}

