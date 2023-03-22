#include "actiongroupwidget.h"
#include "ui_actiongroupwidget.h"

#include <QPaintEvent>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <algorithm>

#include "actionframewidget.h"
#include "distancewidget.h"
#include "commondialog.h"
#include "datacenter.h"
#include "nameinputwidget.h"

double ActionGroupWidget::frameDistance_ = 50.0;
QMap<int,ActionFrameWidget*> ActionGroupWidget::selectedFrameWidgets_;
ActionGroupWidget* ActionGroupWidget::groupOfSelectedFrames_ = nullptr;

ActionGroupWidget::ActionGroupWidget(QString groupName,
                                     QSet<int> frames,
                                     QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActionGroupWidget),
    groupName_(groupName),
    mouseMoved_(false),
    ctrlMotion_(false),
    frameAreaPressed_(false)
{
    ui->setupUi(this);
    init(frames);
}

ActionGroupWidget::~ActionGroupWidget()
{
    delete ui;
}

void ActionGroupWidget::init(QSet<int> frameIndexes)
{
    auto frames = frameIndexes.toList();
    std::sort(frames.begin(),frames.end());
    groupIndex_ = frames.first();
    move(groupIndex_*frameDistance_ - kFrameWidgetWidth/2,0);
    ui->groupName->setText(groupName_);
    groupMenu_ = new QMenu(this);
    copyGroupAction_ = new QAction(tr("copy"),groupMenu_);
    groupMenu_->addAction(copyGroupAction_);
    connect(copyGroupAction_,&QAction::triggered,this,&ActionGroupWidget::onCopy);
    pasteGroupAction_ = new QAction(tr("paste"),groupMenu_);
    groupMenu_->addAction(pasteGroupAction_);
    connect(pasteGroupAction_,&QAction::triggered,this,&ActionGroupWidget::onPaste);
    groupMenu_->addAction(tr("delete"),this,&ActionGroupWidget::onDelete);
    groupMenu_->addAction(tr("rename"),this,&ActionGroupWidget::onRename);
    groupMenu_->addAction(tr("save as template"),this,&ActionGroupWidget::onSaveAsTemplate);

    splitGroupAction_ = new QAction(tr("split"),frameMenu_);
    groupMenu_->addAction(splitGroupAction_);
    connect(splitGroupAction_,&QAction::triggered,this,&ActionGroupWidget::onSplitGroup);

    combineGroupsAction_ = new QAction(tr("combine"),frameMenu_);
    groupMenu_->addAction(combineGroupsAction_);
    connect(combineGroupsAction_,&QAction::triggered,this,&ActionGroupWidget::onCombineGroups);

    frameMenu_ = new QMenu(this);
    copyFrameAction_ = new QAction(tr("copy"),frameMenu_);
    frameMenu_->addAction(copyFrameAction_);
    connect(copyFrameAction_,&QAction::triggered,this,&ActionGroupWidget::onCopyFrame);
    pasteFrameAction_ = new QAction(tr("paste"),frameMenu_);
    frameMenu_->addAction(pasteFrameAction_);
    connect(pasteFrameAction_,&QAction::triggered,this,&ActionGroupWidget::onPasteFrame);
    deleteFrameAction_ = new QAction(tr("delete"),frameMenu_);
    frameMenu_->addAction(deleteFrameAction_);
    connect(deleteFrameAction_,&QAction::triggered,this,&ActionGroupWidget::onDeleteFrame);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&ActionGroupWidget::customContextMenuRequested,this,&ActionGroupWidget::onCustomContextMenuRequested);
    distanceWidget_ = new DistanceWidget(this);
    distanceWidget_->hide();
    addFrames(frames);
    setGroupSelected(false);
}

void ActionGroupWidget::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QWidget::paintEvent(event);

    if(drawSelectionRect_) {
        p.save();
        p.setPen(QColor(0x30,0xE0,0xFE));
        QRect rect(pos1,pos2);
        p.drawRect(rect);
        p.restore();
    }
}

void ActionGroupWidget::addFrame(int frameIndex)
{
    ActionFrameWidget *frameWidget = new ActionFrameWidget(this);
    frameWidget->setFrameIndex(frameIndex);
    frameWidgets_.insert(frameIndex,frameWidget);
    setFixedWidth((frameWidgets_.lastKey() - frameWidgets_.firstKey())*frameDistance_+kFrameWidgetWidth);
    int relativeDistance = (frameIndex - groupIndex_) * frameDistance_;
    frameWidget->move(relativeDistance,33);
    frameWidget->show();
}

void ActionGroupWidget::addFrames(QList<int> frames)
{
    for(auto frameIndex : frames) {
        addFrame(frameIndex);
    }
}

void ActionGroupWidget::showDistanceWidget()
{ 
    auto widget = selectedFrameWidgets_.first();
    auto previousWidget = previousFrameWidget(widget->geometry().left());
    if(previousWidget != nullptr) {
        distanceWidget_->move(previousWidget->geometry().right(),48);
        int distance = widget->geometry().left() - previousWidget->geometry().right();
        distanceWidget_->setFixedWidth(distance);
        distanceWidget_->setDistance(QString::number((distance+kFrameWidgetWidth)/(int)frameDistance_));
        if(!distanceWidget_->isVisible()) {
            distanceWidget_->setVisible(true);
        }
    } else {
        distanceWidget_->setVisible(false);
    }
}

void ActionGroupWidget::resetSelectedFrames()
{
    for(auto frameWidget : frameWidgets_) {
        frameWidget->setFrameSelected(false);
    }
}

void ActionGroupWidget::mousePressEvent(QMouseEvent* event)
{
    auto pos = event->pos();
    groupRightSide_ = geometry().right();
    bool ctrl = event->modifiers().testFlag(Qt::ControlModifier);
    frameAreaPressed_ = !ui->titleWidget->geometry().contains(pos);
    pressedFrameWidget_ = frameWidgetAt(pos);
    pressPos_ = mapToParent(pos);
    mouseMoved_ = false;
    ctrlMotion_ = false;

    if(frameAreaPressed_) {
        if(pressedFrameWidget_ == nullptr) {
            drawSelectionRect_ = true;
            pos1 = pos2 = event->pos();
        } else {
            int frameIndex = pressedFrameWidget_->frameIndex();
            UBTDataCenter->framePressed(frameIndex,ctrl);
        }
    } else { //
        QWidget::mousePressEvent(event);
    }
}


void ActionGroupWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(frameAreaPressed_) {
        bool ctrl = event->modifiers().testFlag(Qt::ControlModifier);
        auto pos = event->pos();
        auto currentPos = mapToParent(pos);
        bool validMotion = (qAbs(currentPos.x() - pressPos_.x()) > 5 || qAbs(currentPos.y() - pressPos_.y()) > 5);

        if(!mouseMoved_ && validMotion) {
            mouseMoved_ = true;
            if(pressedFrameWidget_ != nullptr) {
                int delta = currentPos.x() - pressPos_.x();
                if(ctrl) {
                    ctrlMotion_ = true;
                    for(auto cit = selectedFrameWidgets_.cbegin(); cit != selectedFrameWidgets_.cend(); ++cit) {
                        int frameIndex = cit.key();
                        ActionFrameWidget *frameWidget = new ActionFrameWidget(this);
                        frameWidget->setFrameIndex(frameIndex);
                        tempFrameWidgets_.insert(frameIndex,frameWidget);
                        frameWidget->move((frameIndex-groupIndex_)*frameDistance_,33);
                        frameWidget->show();

                    }
                    UBTDataCenter->copyFramesData();
                    moveFrameWidgets(tempFrameWidgets_,delta);
                } else {
                    moveFrameWidgets(selectedFrameWidgets_,delta);
                    showDistanceWidget();
                    notifyGroupsBehindMove();
                }
            }
        } else if(mouseMoved_){
            if(pressedFrameWidget_ != nullptr) {
                int delta = currentPos.x() - lastPos_.x();
                if(ctrlMotion_) {
                    moveFrameWidgets(tempFrameWidgets_,delta);
                    notifyGroupsBehindMove();
                } else {
                    moveFrameWidgets(selectedFrameWidgets_,delta);
                    showDistanceWidget();
                    notifyGroupsBehindMove();
                }
            } else {
                if(drawSelectionRect_) {
                    pos2 = event->pos();
                    boxChooseFrames();
                    update();
                }
            }
        }
        lastPos_ = currentPos;
    } else {
        QWidget::mouseMoveEvent(event);
    }

}

void ActionGroupWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if(frameAreaPressed_) {
        int frameIndex = (mapToParent(event->pos()).x()+frameDistance_/2)/frameDistance_;
        UBTDataCenter->setCurrentFrameIndex(frameIndex);
        bool ctrl = event->modifiers().testFlag(Qt::ControlModifier);
        frameAreaPressed_ = false;
        distanceWidget_->setVisible(false);
        if(mouseMoved_) {
            if(ctrlMotion_) {
                auto widget = tempFrameWidgets_.first();
                int widgetParentX = mapToParent(widget->pos()).x();
                int newIndex = (widgetParentX + frameDistance_/2)/frameDistance_;

                UBTDataCenter->setCurrentFrameIndex(newIndex);
                UBTDataCenter->pasteCopiedFramesInGroup(groupIndex_,newIndex);
            } else if(mouseMoved_ && pressedFrameWidget_ != nullptr) {
                auto widget = selectedFrameWidgets_.first();
                int widgetParentX = mapToParent(widget->pos()).x();
                int newIndex = (widgetParentX + frameDistance_/2)/frameDistance_;
                int newPosX = newIndex*frameDistance_ - kFrameWidgetWidth/2;
                int delta = newPosX-widgetParentX;
                moveFrameWidgets(selectedFrameWidgets_,delta);

                if(newIndex != widget->frameIndex()) {
                    QMap<int,int> movedFrames;
                    for(auto cit = selectedFrameWidgets_.cbegin();
                        cit != selectedFrameWidgets_.cend();
                        ++cit) {
                        auto tempWidget = cit.value();
                        auto tempIndex = cit.key();
                        int tempNewIndex = (mapToParent(tempWidget->pos()).x()+kFrameWidgetWidth/2)/frameDistance_;
                        movedFrames.insert(tempIndex,tempNewIndex);
                    }
                    UBTDataCenter->moveFrames(movedFrames);
                }
            }
        } else {
            if(pressedFrameWidget_ == nullptr) {
                if(!ctrl) {
                    UBTDataCenter->clearSelection();
                }
            } else {
                if(event->button() == Qt::LeftButton) {
                    UBTDataCenter->frameReleased(pressedFrameWidget_->frameIndex(),ctrl);
                }
            }
        }
        if(drawSelectionRect_) {
            drawSelectionRect_ = false;
            update();
        }
    } else {
        QWidget::mouseReleaseEvent(event);
    }
}

void ActionGroupWidget::notifyGroupsBehindMove()
{
    int groupRigtSide = geometry().right();
    int delta = groupRigtSide - groupRightSide_;
    if(delta != 0) {
        groupRightSide_ = groupRigtSide;
        emit groupWidgetMoved(groupIndex_,delta);
    }
}

int ActionGroupWidget::calcLeftBound()
{
    int leftSide = frameDistance_ - kFrameWidgetWidth/2;
    if(previousGroupWidget_ != nullptr) {
        leftSide = previousGroupWidget_->geometry().right() + frameDistance_ - kFrameWidgetWidth;
    }
    return leftSide;
}

void ActionGroupWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    auto pressedFrameWidget = frameWidgetAt(pos);
    bool titlePressed = ui->titleWidget->geometry().contains(pos);

    if(titlePressed) {
        auto copyType = UBTDataCenter->getCopyType();
        switch(copyType) {
        case DataCenter::kCopyNone:
        case DataCenter::kCopyFrame:
            pasteGroupAction_->setEnabled(false);
            break;
        case DataCenter::kCopyGroup:
            pasteGroupAction_->setEnabled(true);
            break;
        }
        auto selectedGroups = UBTDataCenter->getSelectedGroups();
        if(selectedGroups.size() < 2) {
            combineGroupsAction_->setEnabled(false);
            splitGroupAction_->setEnabled(true);
        } else {
            combineGroupsAction_->setEnabled(true);
            splitGroupAction_->setEnabled(false);
        }
        groupMenu_->exec(mapToGlobal(pos));
    } else {
        auto copyType = UBTDataCenter->getCopyType();
        switch(copyType) {
        case DataCenter::kCopyNone:
        case DataCenter::kCopyGroup:
            pasteFrameAction_->setEnabled(false);
            break;
        case DataCenter::kCopyFrame:
            pasteGroupAction_->setEnabled(true);
            break;
        }
        if(pressedFrameWidget == nullptr) {
            copyFrameAction_->setEnabled(false);
            deleteFrameAction_->setEnabled(false);
        } else {
            copyFrameAction_->setEnabled(true);
            deleteFrameAction_->setEnabled(true);
            pasteFrameAction_->setEnabled(true);
        }
        frameMenu_->exec(mapToGlobal(pos));
    }
}

ActionFrameWidget* ActionGroupWidget::frameWidgetAt(QPoint pos)
{
    for(auto widget : frameWidgets_) {
        if(widget->geometry().contains(pos)) {
            return widget;
        }
    }
    return nullptr;
}

void ActionGroupWidget::onCopy()
{
    DataCenter::getInstance()->copySelectedGroupsData();
}

void ActionGroupWidget::onPaste()
{
    UBTDataCenter->pasetCopiedGroups();
}

void ActionGroupWidget::onDelete()
{
    UBTDataCenter->deleteSelectedGroups();
}

void ActionGroupWidget::onRename()
{
    CommonDialog *dialog = new CommonDialog(tr("Rename"));
    NameInputWidget* widget = new NameInputWidget(tr("New name:"),groupName_,dialog);
    dialog->setDisplayWidget(widget);
    connect(dialog,&CommonDialog::sigAccepted,[this,widget](bool accepted){
        if(accepted) {
            QString newName = widget->newName();
            if(groupName_ != newName) {
                UBTDataCenter->renameGroup(groupIndex_,newName);
                groupName_ = newName;
                ui->groupName->setText(groupName_);
            }
        }
    });
    connect(widget,&NameInputWidget::nameIsEmpty,[dialog](bool isNameEmpty){
        if(isNameEmpty) {
            dialog->onSetOkBtnEnabled(false);
        } else {
            dialog->onSetOkBtnEnabled(true);
        }
    });
    dialog->show();
}

void ActionGroupWidget::onSaveAsTemplate()
{
    UBTDataCenter->saveDataToTemplate(groupIndex_);
}

void ActionGroupWidget::onCopyFrame()
{
    DataCenter::getInstance()->copyFramesData();
}
void ActionGroupWidget::onPasteFrame()
{
    DataCenter::getInstance()->pasteCopiedFrames();
}
void ActionGroupWidget::onDeleteFrame()
{
    DataCenter::getInstance()->deleteSelectedFrames();
}

ActionFrameWidget* ActionGroupWidget::previousFrameWidget(int leftSide)
{
    ActionFrameWidget* previousWidget = nullptr;
    for(auto frameWidget : frameWidgets_) {
        if(selectedFrameWidgets_.values().contains(frameWidget)) {
            continue;
        }
        if(frameWidget->geometry().right() < leftSide) {
            previousWidget = frameWidget;
        } else {
            break;
        }
    }
    return previousWidget;
}

void ActionGroupWidget::setGroupSelected(bool selected)
{
    setProperty("selected",selected);
    style()->unpolish(this);
    style()->polish(this);
    if(selected) {
        ui->selectArea->setPixmap(QPixmap(":/res/image/ic_fram_blue_highlight.png"));
    } else {
        ui->selectArea->setPixmap(QPixmap(":/res/image/ic_fram_blue.png"));
    }
}

bool ActionGroupWidget::containsFrameIndex(int frameIndex)
{
    if(frameWidgets_.empty()) {
        return false;
    }
    int firstKey = frameWidgets_.firstKey();
    int lastKey = frameWidgets_.lastKey();
    if(frameIndex >= firstKey && frameIndex <= lastKey) {
        return true;
    }
    return false;
}

QList<int> ActionGroupWidget::frameList()
{
    return frameWidgets_.keys();
}

int ActionGroupWidget::groupRange()
{
    return (frameWidgets_.lastKey() - frameWidgets_.firstKey() + 1);
}

void ActionGroupWidget::deleteFrameWidget(int frameIndex)
{
    frameWidgets_[frameIndex]->deleteLater();
    frameWidgets_.remove(frameIndex);
}

void ActionGroupWidget::setGroupIndex(int index)
{
    if(groupIndex_ == index) {
        return;
    }
    groupIndex_ = index;
    QMap<int,ActionFrameWidget*> newList;
    int firstIndex = frameWidgets_.firstKey();
    for(auto cit = frameWidgets_.cbegin(); cit != frameWidgets_.cend(); ++cit) {
        int newIndex = cit.key()-firstIndex+index;
        cit.value()->setFrameIndex(newIndex);
        newList.insert(newIndex,cit.value());
    }
    frameWidgets_.swap(newList);
}

void ActionGroupWidget::moveGroupAndOtherFrames(QMap<int,ActionFrameWidget*> frameWidgets, int delta)
{
    move(geometry().left()+delta,0);
    for(auto cit = frameWidgets_.cbegin(); cit != frameWidgets_.cend(); ++cit) {
        auto tempFrameWidget = cit.value();
        if(!frameWidgets.values().contains(tempFrameWidget)) {
            //其他widget要保持不动，就要相对group做相对运动
            tempFrameWidget->move(tempFrameWidget->geometry().left() - delta,33);
        }
    }
}

void ActionGroupWidget::moveFrameWidgets(QMap<int,ActionFrameWidget*> frameWidgets, int delta)
{
    int framesWidth = frameWidgets.last()->geometry().right() -
            frameWidgets.first()->geometry().left() + 1;

    //2. 计算左边界
    int leftBound = calcLeftBound();
    QPoint pos(frameWidgets.first()->geometry().left(),33);

    int framesLeft = mapToParent(pos).x() + delta;
    if(framesLeft < leftBound) {
        framesLeft = leftBound;
    }
    int framesRight = framesLeft + framesWidth;
    int left = framesLeft;
    int right = framesRight;
    for(auto widget : frameWidgets_) {
        if(!frameWidgets.values().contains(widget)) {
            int widgetLeft = mapToParent(widget->pos()).x();
            int widgetRight = widgetLeft + kFrameWidgetWidth;
            if(left > widgetLeft) {
                left = widgetLeft;
            }
            if(right < widgetRight) {
                right = widgetRight;
            }
        }
    }
    setFixedWidth(right - left + 1);
    int moveDelta = left - geometry().left();
    moveGroupAndOtherFrames(frameWidgets,moveDelta);

    if(framesLeft == left) {
        int i = 1;
        int firstFrameIndex = frameWidgets.firstKey();
        for(auto cit = frameWidgets.cbegin(); cit != frameWidgets.cend(); ++cit) {
            if(i == 1) {
                cit.value()->move(0,33);
            } else {
                cit.value()->move((cit.key()-firstFrameIndex)*frameDistance_,33);
            }
            ++i;
        }

    } else {
        int i = 1;
        int firstFramePosX = 0;
        int firstFrameIndex = frameWidgets.firstKey();
        for(auto cit = frameWidgets.cbegin(); cit != frameWidgets.cend(); ++cit) {
            if(i == 1) {
                firstFramePosX = cit.value()->geometry().left()+delta;
                cit.value()->move(firstFramePosX,33);
            } else {
                cit.value()->move(firstFramePosX + (cit.key()-firstFrameIndex)*frameDistance_,33);
            }
            ++i;
        }
    }
}

void ActionGroupWidget::clearSelection()
{
    setGroupSelected(false);
    if(distanceWidget_ && distanceWidget_->isVisible()) {
        distanceWidget_->setVisible(false);
    }
    resetSelectedFrames();
}

void ActionGroupWidget::selectFrameWidget(int frameIndex)
{
    auto widget = frameWidgets_[frameIndex];
    widget->setFrameSelected(true);
    selectedFrameWidgets_.insert(frameIndex,widget);
}

void ActionGroupWidget::resetGroupWidth()
{
    if(frameWidgets_.empty()) {
        return;
    }

    auto widget = frameWidgets_.first();
    int leftSide = widget->geometry().left();
    int rightSide = widget->geometry().right();


    for(auto frameWidget : frameWidgets_) {
        int frameLeftSide = frameWidget->geometry().left();
        int frameRightSide = frameWidget->geometry().right();

        if(frameLeftSide < leftSide) {
            leftSide = frameLeftSide;
        }

        if(frameRightSide > rightSide) {
            rightSide = frameRightSide;
        }

    }

    setFixedWidth(rightSide - leftSide + 1);
}


void ActionGroupWidget::onSplitGroup()
{
    UBTDataCenter->splitGroup();
}

void ActionGroupWidget::onCombineGroups()
{
    UBTDataCenter->combineSelectedGroups();
}

void ActionGroupWidget::boxChooseFrames()
{
    if(drawSelectionRect_) {
        QRect rect(pos1,pos2);
        QSet<int> frames;
        for(auto frame : frameWidgets_) {
            if(frame->geometry().intersects(rect)) {
                frames.insert(frame->frameIndex());
            }
        }
        UBTDataCenter->selectFrames(frames);
    }
}
