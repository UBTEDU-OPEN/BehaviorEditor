#include "behaviortable.h"
#include "ui_behaviortable.h"

#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>

#include "behaviortablewidget.h"
#include "robotcfg.h"
#include "tipwidget.h"
#include "commondefine.h"
#include "combobutton.h"

BehaviorTable::BehaviorTable(QWidget* parent)
    : QWidget(parent)
    ,ui(new Ui::BehaviorTable)
    , m_mirrorTip(nullptr)
    , m_reverseTip(nullptr)
    , m_tipDialog(nullptr)
{
    ui->setupUi(this);

    tableWidget = new BehaviorTableWidget(this);
    ui->verticalLayout->addWidget(tableWidget);

    connect(tableWidget,&BehaviorTableWidget::sigSameServoValue,[this](bool isSame, double value){
        if(isSame){
            ui->servoValue->setEnabled(true);
            ui->servoValue->setText(QString::number(value,'f',1));
        } else {
            ui->servoValue->setEnabled(false);
            ui->servoValue->setText("");
        }
    });
    connect(ui->servoValue,&QLineEdit::editingFinished,[this]{
        QString text = ui->servoValue->text();
        bool ok = false;
        double value = text.toDouble(&ok);
        if(ok){
            tableWidget->onSetServosValue(value);
        }
    });

    m_pAddBodyServoMenu = new QMenu();
    m_pAddAllAction= new QAction(tr("Add All"),m_pAddBodyServoMenu);
    m_pAddBodyServoMenu->addAction(m_pAddAllAction);
    m_pAddBodyServoMenu->addSeparator();
    m_pAddHeadAction= new QAction(tr("Add Head"),m_pAddBodyServoMenu);
    m_pAddBodyServoMenu->addAction(m_pAddHeadAction);
    m_pAddRightArmAction= new QAction(tr("Add Right Arm"),m_pAddBodyServoMenu);
    m_pAddBodyServoMenu->addAction(m_pAddRightArmAction);
    m_pAddLeftArmAction= new QAction(tr("Add Left Arm"),m_pAddBodyServoMenu);
    m_pAddBodyServoMenu->addAction(m_pAddLeftArmAction);
    m_pAddRightLegAction= new QAction(tr("Add Right Leg"),m_pAddBodyServoMenu);
    m_pAddBodyServoMenu->addAction(m_pAddRightLegAction);
    m_pAddLeftLegAction= new QAction(tr("Add Left Leg"),m_pAddBodyServoMenu);
    m_pAddBodyServoMenu->addAction(m_pAddLeftLegAction);
    connect(m_pAddAllAction,&QAction::triggered,this,&BehaviorTable::addAll);
    connect(m_pAddHeadAction,&QAction::triggered,this,&BehaviorTable::addHead);
    connect(m_pAddRightArmAction,&QAction::triggered,this,&BehaviorTable::addRightArm);
    connect(m_pAddLeftArmAction,&QAction::triggered,this,&BehaviorTable::addLeftArm);
    connect(m_pAddRightLegAction,&QAction::triggered,this,&BehaviorTable::addRightLeg);
    connect(m_pAddLeftLegAction,&QAction::triggered,this,&BehaviorTable::addLeftLeg);

    m_pMirrorMenu = new QMenu();
    m_pRightArmToLeft = new QAction(tr("Arm:RightToLeft"),m_pMirrorMenu);
    m_pMirrorMenu->addAction(m_pRightArmToLeft);
    m_pLeftArmToRight = new QAction(tr("Arm:LeftToRight"),m_pMirrorMenu);
    m_pMirrorMenu->addAction(m_pLeftArmToRight);
    m_pRightLegToLeft = new QAction(tr("Leg:RightToLeft"),m_pMirrorMenu);
    m_pMirrorMenu->addAction(m_pRightLegToLeft);
    m_pLeftLegToRight = new QAction(tr("Leg:LeftToRight"),m_pMirrorMenu);
    m_pMirrorMenu->addAction(m_pLeftLegToRight);
    connect(m_pLeftArmToRight,&QAction::triggered,this,&BehaviorTable::mirrorLeftArmToRight);
    connect(m_pRightArmToLeft,&QAction::triggered,this,&BehaviorTable::mirrorRightArmToLeft);
    connect(m_pLeftLegToRight,&QAction::triggered,this,&BehaviorTable::mirrorLeftLegToRight);
    connect(m_pRightLegToLeft,&QAction::triggered,this,&BehaviorTable::mirrorRightLegToLeft);

    m_pReverseMenu = new QMenu();
    m_pReverseFull = new QAction(tr("Full"),m_pReverseMenu);
    m_pReverseMenu->addAction(m_pReverseFull);
    m_pReverseHead = new QAction(tr("Head"),m_pReverseMenu);
    m_pReverseMenu->addAction(m_pReverseHead);
    m_pReverseArm = new QAction(tr("Arm"),m_pReverseMenu);
    m_pReverseMenu->addAction(m_pReverseArm);
    m_pReverseLeg = new QAction(tr("Leg"),m_pReverseMenu);
    m_pReverseMenu->addAction(m_pReverseLeg);
    connect(m_pReverseFull,&QAction::triggered,this,&BehaviorTable::reverseFull);
    connect(m_pReverseHead,&QAction::triggered,this,&BehaviorTable::reverseHead);
    connect(m_pReverseArm,&QAction::triggered,this,&BehaviorTable::reverseArm);
    connect(m_pReverseLeg,&QAction::triggered,this,&BehaviorTable::reverseLeg);

    m_curveMenu = new QMenu(parent);
    m_lineCurveAction = new QAction(tr("Line"),m_curveMenu);
    m_curveMenu->addAction(m_lineCurveAction);
    m_curveMenu->addSeparator();
    m_bezierCurveAction = new QAction(tr("Bezier"),m_curveMenu);
    m_curveMenu->addAction(m_bezierCurveAction);
    ui->motionCurve->setMenu(m_curveMenu);

    connect(m_lineCurveAction,&QAction::triggered,[this]{ onSetCurve(1);});
    connect(m_bezierCurveAction,&QAction::triggered,[this]{ onSetCurve(2);});
    connect(tableWidget,&BehaviorTableWidget::sigSelcectedChanged,this,&BehaviorTable::sigSelcectedChanged);
    connect(tableWidget,&BehaviorTableWidget::sigEnableReverseAndMirror,[this](bool enable){
        ui->reverse->setEnabled(enable);
        ui->mirror->setEnabled(enable);
    });
    connect(tableWidget,&BehaviorTableWidget::sigEnableCurve,[this](bool enable){
        ui->motionCurve->setEnabled(enable);
    });
    connect(tableWidget,&BehaviorTableWidget::sigEnableDelete,[this](bool enable){
       ui->deleteSelection->setEnabled(enable);
    });

    m_mirrorTip = new TipWidget(this);
    connect(m_mirrorTip,&TipWidget::sigEnter,[this]{
        if(m_tipDialog){
            m_tipDialog->close();
            m_tipDialog = nullptr;
        }
        m_tipDialog = new WidgetTipDialog(this);
        QWidget* widget = new QWidget(m_tipDialog);
        QVBoxLayout* layout = new QVBoxLayout(widget);
        QLabel* label = new QLabel(widget);
        label->setText("Mirror");
        QLabel* separator = new QLabel(widget);
        QPixmap pixmap = QPixmap(":/icon/mirror");
        separator->setFixedSize(pixmap.width(),1);
        separator->setStyleSheet("background-color: #191D25;");
        QLabel* description = new QLabel(widget);
        description->setText("mirror robot.");
        QLabel* image = new QLabel(widget);
        image->setPixmap(pixmap);
        layout->addWidget(label);
        layout->addWidget(separator);
        layout->addWidget(description);
        layout->addWidget(image);
        widget->adjustSize();
        m_tipDialog->setDisplayWidget(widget,WidgetTipDialog::TriangleOrientation::DOWN,5);


        auto pos = mapToGlobal(m_mirrorTip->geometry().topLeft());
        pos.setX(pos.x()-m_tipDialog->width()/2);
        pos.setY(pos.y()-m_tipDialog->height()-1);
        m_tipDialog->move(pos);
        m_tipDialog->show();
    });
    connect(m_mirrorTip,&TipWidget::sigLeave,[this]{
        if(m_tipDialog){
            m_tipDialog->close();
            m_tipDialog = nullptr;
        }
    });
    m_reverseTip = new TipWidget(this);
    connect(m_reverseTip,&TipWidget::sigEnter,[this]{
        if(m_tipDialog){
            m_tipDialog->close();
            m_tipDialog = nullptr;
        }
        m_tipDialog = new WidgetTipDialog(this);
        QWidget* widget = new QWidget(m_tipDialog);
        QVBoxLayout* layout = new QVBoxLayout(widget);
        QLabel* label = new QLabel(widget);
        label->setText("Reverse");
        QLabel* separator = new QLabel(widget);
        QPixmap pixmap = QPixmap(":/icon/reverse");
        separator->setFixedSize(pixmap.width(),1);
        separator->setStyleSheet("background-color: #191D25;");
        QLabel* description = new QLabel(widget);
        description->setText("reverse robot.");
        QLabel* image = new QLabel(widget);
        image->setPixmap(pixmap);
        layout->addWidget(label);
        layout->addWidget(separator);
        layout->addWidget(description);
        layout->addWidget(image);
        widget->adjustSize();
        m_tipDialog->setDisplayWidget(widget,WidgetTipDialog::TriangleOrientation::DOWN,5);


        auto pos = mapToGlobal(m_reverseTip->geometry().topLeft());
        pos.setX(pos.x()-m_tipDialog->width()/2);
        pos.setY(pos.y()-m_tipDialog->height()-1);
        m_tipDialog->move(pos);
        m_tipDialog->show();
    });
    connect(m_reverseTip,&TipWidget::sigLeave,[this]{
        if(m_tipDialog){
            m_tipDialog->close();
            m_tipDialog = nullptr;
        }
    });

    connect(tableWidget,&BehaviorTableWidget::sigMirror,this,&BehaviorTable::sigMirror);
    connect(tableWidget,&BehaviorTableWidget::sigReverse,this,&BehaviorTable::sigReverse);

    ui->servoValue->setEnabled(false);
    ui->servoValue->setFixedWidth(100);
    ui->addFrame->setEnabled(false);
    ui->deleteSelection->setEnabled(false);
    ui->mirror->setEnabled(false);
    ui->reverse->setEnabled(false);
    ui->motionCurve->setEnabled(false);

//    ui->addFrame->setPromptMenu(m_pAddBodyServoMenu);
//    ui->addFrame->setFunctionButtonText(tr("Add All"));
//    ui->addFrame->hideSeparator();
    connect(ui->addFrame,&QPushButton::clicked,this,&BehaviorTable::onAddFrameClicked);
    ui->addFrame->setFixedHeight(24);
    configStyle();
}

void BehaviorTable::configStyle()
{
    ui->horizontalLayout_2->setSpacing(12);
}

void BehaviorTable::resizeEvent(QResizeEvent* event)
{
    auto pos = ui->mirror->geometry().topRight();
    pos.setX(pos.x()-m_mirrorTip->width()/2);
    pos.setY(pos.y()-m_mirrorTip->height()/2);
    m_mirrorTip->move(pos);

    pos = ui->reverse->geometry().topRight();
    pos.setX(pos.x()-m_reverseTip->width()/2);
    pos.setY(pos.y()-m_reverseTip->height()/2);
    m_reverseTip->move(pos);
    QWidget::resizeEvent(event);
}

void BehaviorTable::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(childAt(event->pos()) == ui->servoValue){
        ui->servoValue->setEnabled(true);
    }
}

void BehaviorTable::onAddFrameClicked()
{
    addFrame(Robotcfg::EM_BODY_ALL);
}

void BehaviorTable::on_reverse_clicked()
{
    auto rect = ui->reverse->geometry();
    auto globalPos = mapToGlobal(rect.bottomLeft());
    if (m_pReverseMenu)
    {
        m_pReverseMenu->popup(globalPos);
    }
}

void BehaviorTable::on_mirror_clicked()
{
    auto rect = ui->mirror->geometry();
    auto globalPos = mapToGlobal(rect.bottomLeft());
    if (m_pMirrorMenu)
    {
        m_pMirrorMenu->popup(globalPos);
    }
}

void BehaviorTable::onFrameIdChanged(QMap<int, QPair<double, int>> servoInfo, bool isKeyFrame)
{
    Q_UNUSED(servoInfo);
    if(isKeyFrame){
        ui->addFrame->setEnabled(false);
    } else {
        ui->addFrame->setEnabled(true);
    }
}

void BehaviorTable::addFrame(int bodyType)
{
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

void BehaviorTable::addAll()
{
    ui->addFrame->setEnabled(false);
    addFrame(Robotcfg::EM_BODY_ALL);
}
void BehaviorTable::addHead()
{
    addFrame(Robotcfg::EM_BODY_HEAD);
}
void BehaviorTable::addRightArm()
{
    addFrame(Robotcfg::EM_BODY_RIGHT_HAND);
}
void BehaviorTable::addLeftArm()
{
    addFrame(Robotcfg::EM_BODY_LEFT_HAND);
}
void BehaviorTable::addRightLeg()
{
    addFrame(Robotcfg::EM_BODY_RIGHT_LEG);
}
void BehaviorTable::addLeftLeg()
{
    addFrame(Robotcfg::EM_BODY_LEFT_LEG);
}
void BehaviorTable::mirrorLeftArmToRight()
{
    emit sigMirror(UBTYanshee::eMirrorTypeLeftArmToRight);
}
void BehaviorTable::mirrorRightArmToLeft()
{
    emit sigMirror(UBTYanshee::eMirrorTypeRightArmToLeft);
}
void BehaviorTable::mirrorLeftLegToRight()
{
    emit sigMirror(UBTYanshee::eMirrorTypeLeftLegToRight);
}
void BehaviorTable::mirrorRightLegToLeft()
{
    emit sigMirror(UBTYanshee::eMirrorTypeRightLegToLeft);
}

void BehaviorTable::reverseFull()
{
    emit sigReverse(UBTYanshee::eReversalTypeFull);
}
void BehaviorTable::reverseHead()
{
    emit sigReverse(UBTYanshee::eReversalTypeHead);
}
void BehaviorTable::reverseArm()
{
    emit sigReverse(UBTYanshee::eReversalTypeArm);
}
void BehaviorTable::reverseLeg()
{
    emit sigReverse(UBTYanshee::eReversalTypeLeg);
}

void BehaviorTable::on_deleteSelection_clicked()
{
    ui->deleteSelection->setEnabled(false);
    tableWidget->deleteSelection();
}

void BehaviorTable::onSetCurve(int curveType)
{
    tableWidget->onSetCurve(curveType);
}

void BehaviorTable::onBehaviorEditSelected(const QMap<int, QMap<int,double>> &selectedServos)
{
    if(!selectedServos.empty()){
        ui->deleteSelection->setEnabled(true);  
    } else {
        ui->deleteSelection->setEnabled(false);
    }
    tableWidget->onBehaviorEditSelected(selectedServos);
}

void BehaviorTable::onClearSelection()
{
    tableWidget->onClearSelection();
}

void BehaviorTable::onSelectRow(int row)
{
    tableWidget->onSelectRow(row);
}
