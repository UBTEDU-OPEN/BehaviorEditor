#include "servoeditor.h"
#include "ui_servoeditor.h"

#include "robotcfg.h"
#include "servoform.h"
#include "datacenter.h"

ServoEditor::ServoEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServoEditor)
    , currentLimbIndex_(0)
    , maxServoCount_(0)
    , isPlaying_(false)
    , autoSave_(false)
    , handMirrorEnabled_(false)
    , legMirrorEnabled_(false)
{
	ui->setupUi(this);
    createServoUI();

	// 菜单
    this->editMenu_ = new QMenu();
    this->editMenu_->hide();
    this->editFullBodyAction_ = new QAction(tr("Add All"), this->editMenu_);
    this->editMenu_->addAction(this->editFullBodyAction_);
    this->editRightArmAction_ = new QAction(tr("Add Right Arm"), this->editMenu_);
//    this->editMenu_->addAction(this->editRightArmAction_);
    this->editLeftArmAction_ = new QAction(tr("Add Left Arm"), this->editMenu_);
//    this->editMenu_->addAction(this->editLeftArmAction_);
    this->editRightLegAction_ = new QAction(tr("Add Right Leg"), this->editMenu_);
//    this->editMenu_->addAction(this->editRightLegAction_);
    this->editLeftLegAction_ = new QAction(tr("Add Left Leg"), this->editMenu_);
//    this->editMenu_->addAction(this->editLeftLegAction_);
    this->editHeadAction_ = new QAction(tr("Add Head"), this->editMenu_);
//    this->editMenu_->addAction(this->editHeadAction_);
        
	//  按钮
//	ui->cbAddFrame->setFunctionButtonText(tr("Add Modified Servos"));
//    ui->cbAddFrame->setPromptMenu(editMenu_);
    ui->modifyFrame->setFixedHeight(24);
//    ui->modifyFrame->setEnabled(false);
        
    initConnection();
    updateServosInfo();
}

ServoEditor::~ServoEditor(){
	delete ui;
}

void ServoEditor::createServoUI()
{
    const auto& limbs = RobotConfig->getBodys();
    for(auto cit = limbs.cbegin(); cit != limbs.cend(); cit++) {
        int limbId = cit.key();
        auto limb = cit.value();
        switch(cit.key()) {
        case 1:
            ui->qpbHeadPage->setText(limb->getName());
            break;
        case 2:
            ui->qpbRightArmPage->setText(limb->getName());
            break;
        case 3:
            ui->qpbLeftArmPage->setText(limb->getName());
            break;
        case 4:
            ui->qpbRightLegPage->setText(limb->getName());
            break;
        case 5:
            ui->qpbLeftLegPage->setText(limb->getName());
            break;
        default:
            break;
        }
        const auto& servos = limb->getServos();
        int servoCount = servos.size();
        maxServoCount_ = (maxServoCount_ < servoCount ? servoCount : maxServoCount_);
        limbsServoCount_.insert(limbId,servoCount);
    }
    currentLimbIndex_ = limbsServoCount_.cbegin().key();
    for(int i = 0; i < maxServoCount_; i++) {
        ServoForm* servoForm = new ServoForm(this);
        connect(servoForm,&ServoForm::servoAngleChanged,this,&ServoEditor::onServoValueChanged);
        connect(servoForm,&ServoForm::servoAngleChangeFinished,this,&ServoEditor::servoAngleChangeFinished);
        connect(servoForm,&ServoForm::servoLockStateChanged,this,&ServoEditor::onServoLockStateChanged);
        ui->vl_1_3->insertWidget(i+1,servoForm);
        servoWidgets_.push_back(servoForm);
        servoForm->setVisible(false);
    }

    const auto& servos = RobotConfig->getServos();
//    int servoCount = servos.size();
    for(auto cit = servos.cbegin(); cit != servos.cend(); cit++) {
        int servoId = cit.key();
        auto servo = cit.value();
        servosValue_.insert(servoId,servo->getInitAngle());
        servosLockFlag_.insert(servoId,true);
    }
}

void ServoEditor::onLockBtnClicked()
{
    bool limbLockState = isLimbLocked(currentLimbIndex_);
    setLimbLockState(currentLimbIndex_,!limbLockState);
    DataCenter::getInstance()->onChangeLimbLockState(currentLimbIndex_,!limbLockState);
}

void ServoEditor::initConnection()
{
    connect(ui->qpbRightArmPage, SIGNAL(clicked()), this, SLOT(onRightArmClicked()));
    connect(ui->qpbLeftArmPage, SIGNAL(clicked()), this, SLOT(onLeftArmClicked()));
    connect(ui->qpbRightLegPage, SIGNAL(clicked()), this, SLOT(onRightLegClicked()));
    connect(ui->qpbLeftLegPage, SIGNAL(clicked()), this, SLOT(onLeftLegClicked()));
    connect(ui->qpbHeadPage, SIGNAL(clicked()), this, SLOT(onHeadClicked()));

    connect(ui->qpbRightArmPage, &DoubleClickedButton::doubleClicked, [this]{onLockBtnClicked();});
    connect(ui->qpbLeftArmPage, &DoubleClickedButton::doubleClicked, [this]{onLockBtnClicked();});
    connect(ui->qpbRightLegPage, &DoubleClickedButton::doubleClicked, [this]{onLockBtnClicked();});
    connect(ui->qpbLeftLegPage, &DoubleClickedButton::doubleClicked, [this]{onLockBtnClicked();});
    connect(ui->qpbHeadPage, &DoubleClickedButton::doubleClicked, [this]{onLockBtnClicked();});

    connect(ui->autoSave, SIGNAL(clicked()), this, SLOT(onAutoSaveClicked()));
    connect(ui->modifyFrame, &QPushButton::clicked, this, &ServoEditor::onEditFrameBtnClicked);
    connect(ui->addFrame,&QPushButton::clicked,[this]{ onEditAllActionTrigger(); });
    connect(ui->insertFrame,&QPushButton::clicked,[this]{ emit inserFrame(); });

    connect(editFullBodyAction_, SIGNAL(triggered(bool)), this, SLOT(onEditAllActionTrigger()));
    connect(editRightArmAction_, SIGNAL(triggered(bool)), this, SLOT(onEditRightArmActionTrigger()));
    connect(editLeftArmAction_, SIGNAL(triggered(bool)), this, SLOT(onEditLeftArmActionTrigger()));
    connect(editRightLegAction_, SIGNAL(triggered(bool)), this, SLOT(onEditRightLegActionTrigger()));
    connect(editLeftLegAction_, SIGNAL(triggered(bool)), this, SLOT(onEditLeftLegActionTrigger()));
    connect(editHeadAction_, SIGNAL(triggered(bool)), this, SLOT(onEditHeadActionTrigger()));

    connect(ui->lockBtn,&QPushButton::clicked,[this]{
        onLockBtnClicked();
    });
    connect(ui->fullLockBtn,&QPushButton::clicked,[this]{
        bool fullLockState = isFullLocked();
        setFullLockState(!fullLockState);
        DataCenter::getInstance()->onChangeFullLockState(!fullLockState);
    });
    connect(ui->headLockBtn,&QPushButton::clicked,[this]{
        bool headLockState = isHeadLocked();
        setHeadLockState(!headLockState);
        DataCenter::getInstance()->onChangeHeadLockState(!headLockState);
    });
    connect(ui->handsLockBtn,&QPushButton::clicked,[this]{
        bool handsLockState = isHandsLocked();
        setHandsLockState(!handsLockState);
        DataCenter::getInstance()->onChangeHandsLockState(!handsLockState);
    });
    connect(ui->legsLockBtn,&QPushButton::clicked,[this]{
        bool legsLockState = isLegsLocked();
        setLegsLockState(!legsLockState);
        DataCenter::getInstance()->onChangeLegsLockState(!legsLockState);
    });
    connect(ui->mirrorBtn,&QPushButton::clicked,[this]{
        reverseCurrentLimbMirrorEnabled();
        updateServosInfo();
    });

    connect(ui->resetPosture,&QPushButton::clicked,[this]{
        emit resetBtnClicked();
    });
}


void ServoEditor::wheelEvent(QWheelEvent *event){
	if(event->delta() > 0){ // forward
        if(this->currentLimbIndex_ == limbsServoCount_.firstKey()){
			return;
		}else {
            this->currentLimbIndex_--;
		}
	} else if(event->delta() < 0){ // back
        if(this->currentLimbIndex_ == limbsServoCount_.lastKey()){
			return;
		}else {
            this->currentLimbIndex_++;
		}
	}
	updateServosInfo();
}

void ServoEditor::onRightArmClicked(){
    this->currentLimbIndex_ = 2;
	updateServosInfo();
}
void ServoEditor::onLeftArmClicked(){
    this->currentLimbIndex_ = 3;
	updateServosInfo();
}
void ServoEditor::onRightLegClicked(){
    this->currentLimbIndex_ = 4;
	updateServosInfo();
}
void ServoEditor::onLeftLegClicked(){
    this->currentLimbIndex_ = 5;
	updateServosInfo();
}
void ServoEditor::onHeadClicked(){
    this->currentLimbIndex_ = 1;
	updateServosInfo();
}

void ServoEditor::updateServosInfo(){
    auto pBody = RobotConfig->queryBody(currentLimbIndex_);
    if(isFullLocked()) {
        ui->fullLockBtn->setIcon(QIcon(":/res/images/lock.png"));
    } else {
        ui->fullLockBtn->setIcon(QIcon(":/res/images/lock_open.png"));
    }
    if(isLimbLocked(currentLimbIndex_)) {
        ui->lockBtn->setIcon(QIcon(":/res/images/lock.png"));
    } else {
        ui->lockBtn->setIcon(QIcon(":/res/images/lock_open.png"));
    }
    if(isHeadLocked()) {
        ui->headLockBtn->setIcon(QIcon(":/res/images/lock.png"));
    } else {
        ui->headLockBtn->setIcon(QIcon(":/res/images/lock_open.png"));
    }
    if(isHandsLocked()) {
        ui->handsLockBtn->setIcon(QIcon(":/res/images/lock.png"));
    } else {
        ui->handsLockBtn->setIcon(QIcon(":/res/images/lock_open.png"));
    }
    if(isLegsLocked()) {
        ui->legsLockBtn->setIcon(QIcon(":/res/images/lock.png"));
    } else {
        ui->legsLockBtn->setIcon(QIcon(":/res/images/lock_open.png"));
    }
    if(RobotConfig->queryBody(currentLimbIndex_)->getType() == Robotcfg::EM_BODY_HEAD) {
        ui->mirrorBtn->setVisible(false);
    } else {
        ui->mirrorBtn->setVisible(true);
        if(isCurrentLimbMirrorEnabled()) {
            ui->mirrorBtn->setIcon(QIcon(":/res/images/mirror.png"));
        } else {
            ui->mirrorBtn->setIcon(QIcon(":/res/images/unmirror.png"));
        }
    }

    auto servos = pBody->getServos();
    int i = 0;
    for(auto cit = servos.cbegin(); cit != servos.cend(); cit++,i++){
		int servoId = cit.key();
		auto servo = cit.value();
        auto servoWidget = servoWidgets_[i];
        servoWidget->setId(servoId);
        servoWidget->setName(servo->getName());
        servoWidget->setMaxValue(servo->getMaxAngle());
        servoWidget->setMinValue(servo->getMinAngle());
        servoWidget->setValue(servosValue_[servoId]);
        servoWidget->setLockState(servosLockFlag_[servoId]);
        servoWidget->setVisible(true);
	}
        
    for(; i < maxServoCount_; i++) servoWidgets_[i]->setVisible(false);
        
	// 切换样式
    QString qsNormalStyle("QPushButton{"
                  "font-size: 12px;"
                  "font-family: SourceHanSansCN-Regular;"
                  "background-color: #2C313D;"
                  "background-image: url(:/res/images/servo_tabbutton_bg.png);"
                  "}"
                  "QPushButton:hover{"
                  "color: #5DFCFF;"
                  "background-image: url(:/res/images/servo_tabbutton_bg_hovering.png);"
                  "}"
                  );
    QString qsSelectedStyle("QPushButton{"
                "font-size: 12px;"
                "font-family: SourceHanSansCN-Regular;"
                "color: #5DFCFF;"
                "background-color: #2C313D;"
                "background-image: url(:/res/images/servo_tabbutton_bg_hovering.png);"
                "}");
	ui->qpbHeadPage->setStyleSheet(qsNormalStyle);
	ui->qpbRightArmPage->setStyleSheet(qsNormalStyle);
	ui->qpbLeftArmPage->setStyleSheet(qsNormalStyle);
	ui->qpbRightLegPage->setStyleSheet(qsNormalStyle);
	ui->qpbLeftLegPage->setStyleSheet(qsNormalStyle);
    switch(this->currentLimbIndex_){
	case 1: ui->qpbHeadPage->setStyleSheet(qsSelectedStyle); break;
	case 2: ui->qpbRightArmPage->setStyleSheet(qsSelectedStyle); break;
	case 3: ui->qpbLeftArmPage->setStyleSheet(qsSelectedStyle); break;
	case 4: ui->qpbRightLegPage->setStyleSheet(qsSelectedStyle); break;
	case 5: ui->qpbLeftLegPage->setStyleSheet(qsSelectedStyle); break;
	}
}

void ServoEditor::onEditFrameBtnClicked(){
    emit editFrameBtnClicked();
//    ui->modifyFrame->setEnabled(false);
}

void ServoEditor::onServoValueChanged(int servoId, double value){
    servosValue_[servoId] = value;
    if(!autoSave_) {
        ui->modifyFrame->setEnabled(true);
    }
        
	// 镜像处理
    if(isCurrentLimbMirrorEnabled()) {
        int oppositeServoId = servoId;
        double oppositeServoAngle = value;
        RobotConfig->calcOppositeServoValue(oppositeServoId,oppositeServoAngle);
        servosValue_[oppositeServoId] = oppositeServoAngle;
        //TODO: 需要合并接口，要不然机器人可能处理不过来
        emit servoAngleChanged(servoId,value);
        emit servoAngleChanged(oppositeServoId,oppositeServoAngle);
    } else {
        emit servoAngleChanged(servoId,value);
    }
}

void ServoEditor::onServoLockStateChanged(int id, bool isLocked)
{
    servosLockFlag_[id] = isLocked;
    updateServosInfo();
    DataCenter::getInstance()->onChangeServoLockState(id,isLocked);
}

void ServoEditor::onEditActionTriggered(int bodyType){
    int addType = 0;
    int bodyId = 0;
    if(bodyType == Robotcfg::EM_BODY_ALL) {
        addType = Robotcfg::EM_ADD_ALL;
    } else {
        addType = Robotcfg::EM_ADD_BODY;
        bodyId = RobotConfig->queryBodyIdByBodyType(bodyType);
    }
    emit sigAddFrame(addType, bodyId);
}

void ServoEditor::onEditAllActionTrigger(){
	onEditActionTriggered(Robotcfg::EM_BODY_ALL);
}

void ServoEditor::onEditRightArmActionTrigger(){
	onEditActionTriggered(Robotcfg::EM_BODY_RIGHT_HAND);
}
void ServoEditor::onEditLeftArmActionTrigger(){
	onEditActionTriggered(Robotcfg::EM_BODY_LEFT_HAND);
}
void ServoEditor::onEditRightLegActionTrigger(){
	onEditActionTriggered(Robotcfg::EM_BODY_RIGHT_LEG);
}
void ServoEditor::onEditLeftLegActionTrigger(){
	onEditActionTriggered(Robotcfg::EM_BODY_LEFT_LEG);
}
void ServoEditor::onEditHeadActionTrigger(){
	onEditActionTriggered(Robotcfg::EM_BODY_HEAD);
}

void ServoEditor::onFrameIdChanged(QMap<int, QPair<double, int>> servoInfo, bool isKeyFrame){
	if(servoInfo.empty() || !isKeyFrame){
        ui->modifyFrame->setEnabled(true);
	}
	for(auto cit = servoInfo.cbegin(); cit != servoInfo.cend(); cit++){
        this->servosValue_[cit.key()] = cit.value().first;
	}
    if(!isPlaying_) {
        updateServosInfo();
    }
}

void ServoEditor::onSingleServoAngleChanged(int id, double value){
    this->currentLimbIndex_ = RobotConfig->queryBodyIdByServoId(id);
    this->servosValue_[id] = value;
    if(!autoSave_) {
        ui->modifyFrame->setEnabled(true);
    }
	updateServosInfo();
}

void ServoEditor::onServoSelectinChanged(int nId, int nBody){
	Q_UNUSED(nId);
    this->currentLimbIndex_ = nBody;
	updateServosInfo();
}

void ServoEditor::onPlayingStatusChanged(bool isPlaying)
{
    isPlaying_ = isPlaying;
}

void ServoEditor::setFullLockState(bool isLocked)
{
    for(auto& servoLockState : servosLockFlag_) {
        servoLockState = isLocked;
    }
    updateServosInfo();
}

void ServoEditor::setLimbLockState(int limbId, bool isLocked)
{
    auto servoIds = RobotConfig->queryBody(limbId)->getServos().keys();
    for(int servoId : servoIds) {
        servosLockFlag_[servoId] = isLocked;
    }
    updateServosInfo();
}

bool ServoEditor::isFullLocked()
{
    return (servosLockFlag_.values().contains(false) ? false : true);
}

bool ServoEditor::isLimbLocked(int limbId)
{
    auto servoIds = RobotConfig->queryBody(limbId)->getServos().keys();
    for(int servoId : servoIds) {
        if(servosLockFlag_[servoId] == false) {
            return false;
        }
    }
    return true;
}

void ServoEditor::onServosAutoLocked(QSet<int> servoIds)
{
    for(int servoId : servoIds) {
        servosLockFlag_[servoId] = true;
    }
    updateServosInfo();
}

void ServoEditor::onResetMirrorState()
{
    handMirrorEnabled_ = false;
    legMirrorEnabled_ = false;
    updateServosInfo();
}

bool ServoEditor::isHeadLocked()
{
    int headIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_HEAD);
    return isLimbLocked(headIndex);
}

bool ServoEditor::isHandsLocked()
{
    int rightHandIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_RIGHT_HAND);
    int leftHandIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_LEFT_HAND);
    bool ret = isLimbLocked(rightHandIndex) && isLimbLocked(leftHandIndex);
    return ret;
}

bool ServoEditor::isLegsLocked()
{
    int rightLegIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_RIGHT_LEG);
    int leftLegIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_LEFT_LEG);
    bool ret = isLimbLocked(rightLegIndex) && isLimbLocked(leftLegIndex);
    return ret;
}

void ServoEditor::setHeadLockState(bool isLocked)
{
    int headIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_HEAD);
    setLimbLockState(headIndex,isLocked);
}
void ServoEditor::setHandsLockState(bool isLocked)
{
    int rightHandIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_RIGHT_HAND);
    int leftHandIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_LEFT_HAND);
    setLimbLockState(rightHandIndex,isLocked);
    setLimbLockState(leftHandIndex,isLocked);
}
void ServoEditor::setLegsLockState(bool isLocked)
{
    int rightLegIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_RIGHT_LEG);
    int leftLegIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_LEFT_LEG);
    setLimbLockState(rightLegIndex,isLocked);
    setLimbLockState(leftLegIndex,isLocked);
}

bool ServoEditor::isCurrentLimbMirrorEnabled()
{
    auto limbType = RobotConfig->queryBody(currentLimbIndex_)->getType();
    if(limbType == Robotcfg::EM_BODY_RIGHT_HAND || limbType == Robotcfg::EM_BODY_LEFT_HAND) {
        return handMirrorEnabled_;
    } else if(limbType == Robotcfg::EM_BODY_RIGHT_LEG || limbType == Robotcfg::EM_BODY_LEFT_LEG) {
        return legMirrorEnabled_;
    }
    return false;
}
void ServoEditor::reverseCurrentLimbMirrorEnabled()
{
    auto limbType = RobotConfig->queryBody(currentLimbIndex_)->getType();
    if(limbType == Robotcfg::EM_BODY_RIGHT_HAND || limbType == Robotcfg::EM_BODY_LEFT_HAND) {
        handMirrorEnabled_ = !handMirrorEnabled_;
        emit handsMirrorStateChanged(handMirrorEnabled_);
    } else if(limbType == Robotcfg::EM_BODY_RIGHT_LEG || limbType == Robotcfg::EM_BODY_LEFT_LEG) {
        legMirrorEnabled_ = !legMirrorEnabled_;
        emit legsMirrorStateChanged(legMirrorEnabled_);
    }
}
