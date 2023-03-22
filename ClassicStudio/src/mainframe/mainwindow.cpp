#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QMouseEvent>
#include <QDebug>
#include <QToolButton>
#include <QIcon>
#include <QPushButton>
#include <QTabBar>
#include <QLabel>
#include <QDesktopServices>

#include "titlebar.h"
#include "robotview.h"
#include "behavioredit.h"

#include "behaviortable.h"
#include "widgettipdialog.h"
#include "datacenter.h"
#include "robotcfg.h"
#include"robotsearchdialog.h"
#include <QtPlatformHeaders/QWindowsWindowFunctions>
#include "jointadjuestmentwidget.h"
#include"virtualrobotwidget.h"
#include "commondialog.h"
#include "logHelper.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_bWndMax(false),
      m_bLeftButtonPressed(false),
      behaviorEdit_(nullptr),
      m_lastPos(0, 0),
      m_dontShowUpdateWindow(false)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::init(){
    initWindows();
    initConnect();
    //初始化后创建data
    UBTDataCenter->newBehaviorData();
//	autoCheckForUpdates();
}

void MainWindow::initWindows(){
    setWindowFlags(Qt::FramelessWindowHint);

    auto templateTab = new QTabWidget(this);
    templateTab->setObjectName("templateTab");
    QFile styleSheet(":/qss/res/qss/template.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        ui->templateBarHolder->setStyleSheet(styleSheet.readAll());
    }
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0,6,0,0);
    hLayout->addWidget(templateTab);
    ui->templateBarHolder->setLayout(hLayout);
    ui->templateBarHolder->setFixedWidth(340);
    templateBar = new TemplateBar(this);
    templateTab->addTab(templateBar,tr("Template"));
    robotActionList = new RobotActionList(this);
    templateTab->addTab(robotActionList,tr("Action List"));

    ui->behaviorEditHolder->setLayout(new QVBoxLayout);
    ui->behaviorEditHolder->layout()->setContentsMargins(0,0,0,0);
    ui->behaviorEditHolder->layout()->setSpacing(2);
    QWidget* timeShaftTitle = new QWidget(ui->behaviorEditHolder);
    timeShaftTitle->setObjectName("timeShaftTitle");
    timeShaftTitle->setFixedHeight(36);
    timeShaftTitle->setLayout(new QHBoxLayout);
    timeShaftTitle->layout()->addWidget(new QLabel(tr("Time Shaft"),timeShaftTitle));
    auto margin = timeShaftTitle->layout()->contentsMargins();
    margin.setLeft(20);
    timeShaftTitle->layout()->setContentsMargins(margin);
    ui->behaviorEditHolder->layout()->addWidget(timeShaftTitle);

    behaviorEdit_ = new BehaviorEdit;
    ui->behaviorEditHolder->layout()->addWidget(behaviorEdit_);

    mTemplateBarLabel = new QLabel(this);
    QRect qRect= mTemplateBarLabel->geometry();
    mTemplateBarLabel->setGeometry(qRect.x(),qRect.y(),24,24);
    mTemplateBarLabel->setStyleSheet("background-image: url(:/image/res/image/template/ic_file_mouseover.png);");
    mTemplateBarLabel->hide();
    configStyle();
}

void MainWindow::configStyle(){
    //	ui->previewHolder->layout()->setContentsMargins(0, 4, 0, 0);
    ui->previewHolder->setFixedWidth(1000);
    ui->servoEditPane->setFixedWidth(570);
    //	ui->behaviorEditHolder->layout()->setContentsMargins(0, 4, 0, 0);
    ui->horizontalLayout->setSpacing(2);
    ui->verticalLayout->setSpacing(2);
    //	ui->verticalLayout_2->setSpacing(2);
}

void MainWindow::addPreviewTab(int index){
    auto btn = new QPushButton(this);
    btn->setIcon(QIcon(":/image/res/image/icon_magnify.png"));
    connect(
                btn, &QPushButton::clicked, [ = ] {
        onZoomTabWidget(index);
    });
    m_previewTab->tabBar()->setTabButton(index, QTabBar::ButtonPosition::RightSide, btn);
}

void MainWindow::initConnect(){
    /**< 关联标题信号 */
    connect(ui->titleBar, &TitleBar::sigClose, this, &MainWindow::onClose);
    connect(ui->titleBar, &TitleBar::sigMax, this, &MainWindow::onMax);
    connect(ui->titleBar, &TitleBar::sigMin, this, &MainWindow::onMin);

    connect(ui->toolBar, &ToolBar::sigNewFile, this, &MainWindow::onNewFile);
    connect(ui->toolBar, &ToolBar::sigOpenFile, this, &MainWindow::onOpenFile);
    connect(ui->toolBar, &ToolBar::sigSaveFile, this, &MainWindow::onSaveFile);
    connect(ui->toolBar, &ToolBar::sigSaveAs, this, &MainWindow::onSaveAs);
    connect(ui->toolBar, &ToolBar::sigHelp, this, &MainWindow::onHelp);
//    connect(ui->toolBar,&ToolBar::sigHelp,[this]{
//        CommonDialog *dialog = new CommonDialog(tr("version"),CommonDialog::OnlyOkButton,this);
//        dialog->setDisplayText(tr("version:")+qApp->applicationVersion());
//        dialog->setAbandonBtnText(tr("I Known"));
//        dialog->show();
//    });




    /** 虚拟机器人界面信号处理
        previewHolder 机器人虚拟界面
        servoEditPane 舵机调节界面
        templateBarHolder 模板和动作列表界面
    **/
    connect(ui->previewHolder,&VirtualRobotWidget::sigCurrentSelectServoId,ui->servoEditPane,&JointAdjuestmentWidget::onSigCurrentSelectServoId);
    connect(ui->servoEditPane,&JointAdjuestmentWidget::servoAngleChanged,DataCenter::getInstance(),&DataCenter::changeCurrentFrameServoNotToRobot);

    connect(ui->previewHolder, &VirtualRobotWidget::sigServoAngleChange, DataCenter::getInstance(), &DataCenter::changeCurrentFrameServo);

    connect(DataCenter::getInstance(), &DataCenter::currentFrameServoChanged,ui->previewHolder,&VirtualRobotWidget::onJointVauleChange);

    connect(ui->servoEditPane,&JointAdjuestmentWidget::servoAngleChangeFinished,DataCenter::getInstance(),&DataCenter::servoAngleChangeFinished);

    connect(ui->servoEditPane,&JointAdjuestmentWidget::sigServoSelected,ui->previewHolder,&VirtualRobotWidget::onServoSelected);


    connect(ui->previewHolder,&VirtualRobotWidget::resetBtnClicked,DataCenter::getInstance(),&DataCenter::resetPosture);

    connect(
                DataCenter::getInstance(), &DataCenter::changeFrameData,
                ui->servoEditPane, &JointAdjuestmentWidget::onFrameIdChanged);
    connect(
                DataCenter::getInstance(), &DataCenter::changeFrameData,
                ui->previewHolder, &VirtualRobotWidget::onSetFramAglne);
    connect(
        DataCenter::getInstance(), &DataCenter::changeFrameDataWithRunTime,
        ui->previewHolder, &VirtualRobotWidget::onSetFramAngleWithRunTime);

    //connect(ui->servoEditPane,&JointAdjuestmentWidget::servoAngleChangeFinished,DataCenter::getInstance(),&DataCenter::servoAngleChangeFinished);
   connect(ui->previewHolder,&VirtualRobotWidget::modify,DataCenter::getInstance(),&DataCenter::onApplyCurrentFrameChanges);

    // dockwidget -> behaviorEdit
    connect(templateBar, &TemplateBar::sigAddCurFrame, behaviorEdit_, &BehaviorEdit::addFileToCurrentFrame);
    connect(templateBar,&TemplateBar::sigItemLongPress,this,&MainWindow::onTemplateBarItemLongPress);
    connect(templateBar,&TemplateBar::sigItemLongPressMove,this,&MainWindow::onTemplateBarItemLongPressMove);
    connect(templateBar,&TemplateBar::sigItemLongPressRelease,this,&MainWindow::onTemplateBarItemLongPressRelease);

    /*****************************************************************************************************/


    /**< 发送对象m_pRobotGlView  */

    //    connect(
    //        DataCenter::getInstance(), &DataCenter::robotModelChangeServo,
    //        ui->servoEditPane, &ServoEditor::onSingleServoAngleChanged);
    //    connect(m_pRobotGlView, &QRobotView::sigCurrentSelectServoId, ui->servoEditPane, &ServoEditor::onServoSelectinChanged);




    connect(&m_updater, &UpdaterWindow::dontShowUpdateWindowChanged, [ = ](bool state){ m_dontShowUpdateWindow = state; });

    //datacenter

    connect(ui->toolBar,&ToolBar::robotBtnClicked,this,&MainWindow::onRobotBtnClicked);
    connect(DataCenter::getInstance(),&DataCenter::robotConnectionStateChanged,ui->toolBar,&ToolBar::connectionStateChanged);
    connect(DataCenter::getInstance(),&DataCenter::sigAddMusic,behaviorEdit_,&BehaviorEdit::addMusic);

    connect(DataCenter::getInstance(),&DataCenter::robotActionList,[this](const QList<QString>& actionList){
        robotActionList->createNewList(actionList);
    });

    connect(DataCenter::getInstance(),&DataCenter::currentFrameIndexChanged,behaviorEdit_,&BehaviorEdit::onChangeFrameIndex);
    connect(DataCenter::getInstance(),&DataCenter::resetLockState,ui->servoEditPane,&JointAdjuestmentWidget::resetLockState);
    connect(DataCenter::getInstance(),&DataCenter::resetMirrorState,ui->servoEditPane,&JointAdjuestmentWidget::resetMirrorState);

    connect(ui->servoEditPane,&JointAdjuestmentWidget::handsMirrorStateChanged,DataCenter::getInstance(),&DataCenter::onHandsMirrorStateChanged);
    connect(ui->servoEditPane,&JointAdjuestmentWidget::legsMirrorStateChanged,DataCenter::getInstance(),&DataCenter::onLegsMirrorStateChanged);

    connect(DataCenter::getInstance(),&DataCenter::chargingStateNotify,[this](int inCharging){
        ui->toolBar->chargingStateChanged(inCharging);
    });
    connect(DataCenter::getInstance(),&DataCenter::batteryPercentNotify,[this](int percent){
        ui->toolBar->batteryPercentChanged(percent);
    });
    connect(DataCenter::getInstance(),&DataCenter::volumeNotify,[this](int volume){
        ui->toolBar->volumeChanged(volume);
    });

    //    connect(DataCenter::getInstance(),&DataCenter::refreshTemplate,templateBar,&TemplateBar::initVariable);

    connect(DataCenter::getInstance(),&DataCenter::totalTimeChanged,behaviorEdit_,&BehaviorEdit::onTotalTimeChanged);
    connect(UBTDataCenter,&DataCenter::postureReset,ui->servoEditPane,&JointAdjuestmentWidget::onPostureReset);
    connect(UBTDataCenter,&DataCenter::postureReset,ui->previewHolder,&VirtualRobotWidget::onPostureReset);

    connect(UBTDataCenter,&DataCenter::fileEdited,ui->titleBar,&TitleBar::onEdited);
    connect(UBTDataCenter,&DataCenter::fileOpend,ui->titleBar,&TitleBar::onOpend);
    connect(UBTDataCenter,&DataCenter::fileSaved,ui->titleBar,&TitleBar::onSaved);
    connect(UBTDataCenter,&DataCenter::frameDivisionChanged,behaviorEdit_,&BehaviorEdit::onFrameDivisionChanged);
    connect(UBTDataCenter,&DataCenter::addFileToTemplate,[this](const QString& fileName){
        templateBar->addItem(fileName);
    });
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton &&
            ui->titleBar->rect().contains(event->pos())){
        m_lastPos = event->globalPos();
        m_bLeftButtonPressed = true;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && m_bLeftButtonPressed){
        m_bLeftButtonPressed = false;
    }
    if(!mTemplateBarLabel->isHidden()){
        mTemplateBarLabel->move(0,0);
        mTemplateBarLabel->hide();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons().testFlag(Qt::LeftButton) &&  m_bLeftButtonPressed){
        QPoint position = pos() + event->globalPos() - m_lastPos;
        move(position.x(), position.y());
        m_lastPos = event->globalPos();
        templateBar->adjustDockWidget();
    }
    if(event->button() == Qt::LeftButton){
        if(!mTemplateBarLabel->isHidden()){
            QPoint point =  event->globalPos();
            mTemplateBarLabel->move(point.x()-12,point.y() - 12 );
        }
    }
}

void MainWindow::onMax(){
    if(m_bWndMax){
        m_bWndMax = false;
        this->showNormal();
    } else {
        m_bWndMax = true;
        this->showMaximized();
    }
}

void MainWindow::onMin(){
    this->showMinimized();
}

void MainWindow::onClose(){
    if(UBTDataCenter->isFileEdited()) {
        CommonDialog* dialog = new CommonDialog(tr("System Prompt"),CommonDialog::SaveButton);
        dialog->setDisplayText(tr("File edited, are you sure close?"));
        bool isCancel = false;
        connect(dialog,&CommonDialog::sigClose,[&isCancel]{
            isCancel = true;
        });
        connect(dialog,&CommonDialog::sigSave,[](bool save){
            if(save) {
                UBTDataCenter->saveBehaviorData();
            }
        });
        dialog->exec();
        if(isCancel) {
            return;
        }
    }
    close();
}

void MainWindow::onNewFile(){
    UBTDataCenter->resetPosture();
    UBTDataCenter->newBehaviorData();
}

void MainWindow::onOpenFile(){
    UBTDataCenter->resetPosture();
    auto filePath = QFileDialog::getOpenFileName(this, "Open", qApp->applicationDirPath(), "Behavior Files(*.xml *.hts)");
    if(filePath.isEmpty()){
        LOG(ERROR) << "Open file path is empty.";
        return;
    }
    DataCenter::getInstance()->loadBehaviorData(filePath);
}

void MainWindow::onSaveFile(){
    DataCenter::getInstance()->saveBehaviorData(false);
}

void MainWindow::onSaveAs(){
    DataCenter::getInstance()->saveBehaviorData(true);
}

void MainWindow::onHelp()
{
    QString filePath = QApplication::applicationDirPath() + "/resource/help.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void MainWindow::onZoomTabWidget(int index){
    //	QWidget *pTabWidget = m_previewTab->widget(index);
    //	if(pTabWidget == nullptr){
    //		return;
    //	}
    //	QString strTabTitle = m_previewTab->tabText(index);
    //	m_previewTab->removeTab(index);
    //	pTabWidget->setVisible(true);

    //	QHBoxLayout *hlayout = new QHBoxLayout;
    //	hlayout->setMargin(0);
    //	hlayout->setSpacing(0);
    //	hlayout->setContentsMargins(QMargins(0, 0, 0, 0));
    //	hlayout->addWidget(pTabWidget);

    //	QDialog *pShow3dModel = new QDialog;
    //	pShow3dModel->setWindowTitle(strTabTitle);
    //	pShow3dModel->setWindowFlags(
    //		Qt::Dialog |
    //		Qt::WindowTitleHint |
    //		Qt::CustomizeWindowHint |
    //		Qt::WindowCloseButtonHint);
    //	pShow3dModel->setLayout(hlayout);
    //	pShow3dModel->resize(800, 600);
    //	connect(
    //		pShow3dModel, &QDialog::rejected, this, [ = ]() mutable {
    //		m_previewTab->insertTab(index, pTabWidget, strTabTitle);
    //		m_previewTab->setCurrentIndex(index);
    //		pTabWidget->setVisible(true);

    //		pShow3dModel->close();
    //		pShow3dModel->deleteLater();
    //		pShow3dModel = nullptr;
    //		addPreviewTab(index);
    //	});
    //	pShow3dModel->setModal(false);
    //	pShow3dModel->show();
}

void MainWindow::onLayerChanged(int layer){
    //TODO:
}

void MainWindow::onUpdate(){
    m_updater.checkForUpdates(true);
}



QString MainWindow::getFileName(const QString &title){
    auto filePath = QFileDialog::getSaveFileName(
                this,
                title,
                qApp->applicationDirPath(),
                "",
                nullptr,
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(filePath.isEmpty()){
        return "";
    }
    QFileInfo file(filePath);
    if(file.exists()){
        return "";
    }
    file.dir().mkdir(filePath);
    QString fileName = file.fileName();
    filePath += "/" + fileName + ".xml";
    return filePath;
}

/**
 * Checks for updates, if an update is found, then the updater dialog will show
 * up, otherwise, no notification shall be showed
 */
void MainWindow::autoCheckForUpdates(){
    m_updater.installEventFilter(this);
    m_updater.setShowWindowDisable(m_dontShowUpdateWindow);
    m_updater.checkForUpdates(false);
}

/**
 * @brief
 * Mostly take care on the event happened on widget whose filter installed to tht mainwindow
 */
bool MainWindow::eventFilter(QObject *object, QEvent *event){
    switch(event->type()){
    case QEvent::Show:
        if(object == &m_updater){
            QRect rect = m_updater.geometry();
            QRect appRect = geometry();
            int titleBarHeight = 28;

            int x = appRect.x() + (appRect.width() - rect.width()) / 2.0;
            int y = appRect.y() + titleBarHeight  + (appRect.height() - rect.height()) / 2.0;

            m_updater.setGeometry(QRect(x, y, rect.width(), rect.height()));
        }
        break;
    case QEvent::ToolTip:
    {
        QString text;
        if(object->objectName() == "WatchBtn" ||
                object->objectName() == "ServoSyncBtn" ||
                object->objectName() == "StanceBtn"){
            text = dynamic_cast<QWidget*>(object)->toolTip();
        }
        if(text != ""){
            WidgetTipDialog *dialog = new WidgetTipDialog(this);
            dialog->setDisplayText(text, WidgetTipDialog::TriangleOrientation::UP, 5);
            auto pos = static_cast<QHelpEvent*>(event)->globalPos();
            pos.setX(pos.x() - dialog->width() / 2);
            pos.setY(pos.y() + 10);
            dialog->move(pos);
            dialog->show();
            return true;
        }
    }
        break;
    default:
        break;
    }

    return QObject::eventFilter(object, event);
}

//连接机器人按钮被点击了
void MainWindow::onRobotBtnClicked()
{
    //    RobotSearchDialog *robotdialog = new RobotSearchDialog();
    //    connect(robotdialog,&RobotSearchDialog::connectStateChanged,ui->toolBar,&ToolBar::connectionStateChanged);
    //    robotdialog->exec();

    DataCenter::getInstance()->connectRobot();
}

void MainWindow::onTemplateBarItemLongPress(QPoint point, QString filePath)
{
    mTemplateBarLabel->show();
    mTemplateBarLabel->move(point.x()-12, point.y()-12 );

}

void MainWindow::onTemplateBarItemLongPressRelease(QPoint point,QString filePath)
{
    if(!mTemplateBarLabel->isHidden()){
        mTemplateBarLabel->setStyleSheet("background-image: url(:/image/res/image/template/ic_file_mouseover.png);");
        mTemplateBarLabel->move(0,0);
        mTemplateBarLabel->hide();
        QRect qRect = behaviorEdit_->getBehaviorLayerRect();
        if(qRect.contains(point)){
            behaviorEdit_->addFileToCurrentFrame(filePath);
        }
    }
}

void MainWindow::onTemplateBarItemLongPressMove(QPoint point)
{
    if(!mTemplateBarLabel->isHidden()){
        mTemplateBarLabel->move(point.x()-12,point.y()-12);
        QRect qRect = behaviorEdit_->getBehaviorLayerRect();
        if(qRect.contains(point)){
            mTemplateBarLabel->setStyleSheet("background-image: url(:/image/res/image/template/ic_file_p.png);");
        }else {
            mTemplateBarLabel->setStyleSheet("background-image: url(:/image/res/image/template/ic_file_mouseover.png);");
        }
    }
}

void MainWindow::onCtrlKeyEvent(int keyEvent)
{
    qDebug() << "MainWindow::onCtrlKeyEvent" << (Qt::Key)keyEvent;
}

void MainWindow::onNormalKeyEvent(int keyEvent)
{
    qDebug() << "MainWindow::onNormalKeyEvent" << (Qt::Key)keyEvent;
}

