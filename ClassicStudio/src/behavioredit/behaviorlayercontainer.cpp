#include "behaviorlayercontainer.h"

#include <QVBoxLayout>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QPaintEvent>
#include <QDrag>
#include <QDropEvent>
#include <QMimeData>
#include <QByteArray>
#include <QVariant>
#include <QApplication>
#include <QDataStream>
#include <QScrollArea>
#include <QUrl>
#include <QtMath>
#include <QFileDialog>
#include <QMediaContent>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QWidget>
#include <QMediaPlayer>
#include <QDir>
#include <thread>
#include <QTextCodec>

#include "frameintervaleditor.h"
#include "labelblock.h"
#include "audio.h"
#include "robotcfg.h"
#include "commondefine.h"
#include "widgettipdialog.h"
#include "datacenter.h"
#include "actiongroupwidget.h"
#include "commondialog.h"
#include "commondefine.h"

const int LAYER_WIDGET_HEIGHT = 96;

const QString BehaviorLayerContainer::kServoItemDataFormat {"servo"};
const QString BehaviorLayerContainer::kMusicItemDataFormat {"music"};
const QString BehaviorLayerContainer::kLEDItemDataFormat {"LED"};

BehaviorLayerContainer::BehaviorLayerContainer(int fn, double fd, QWidget *parent)
    : QWidget(parent)
    , frameNumber_(fn)
    , frameDistanceBase_(fd)
    , scaleFactor_(1)
    , audioPlayTimer_(nullptr)
    , mediaPlayer_(nullptr)
    , musicWidget_(nullptr)
    , selectedFrameIndex_(0)
    , selectionType_(kSelectionNone)
    , mouseMoved_(false)
    , pressedGroupWidget_(nullptr)
    , ctrlMotion_(false)
    , mousePressed_(false)
{
    initConfiguration();
    init();
    updateContent();

    //TODO:
//    connect(UBTDataCenter,&DataCenter::groupChanged,this,&BehaviorLayerContainer::onGroupChanged);
    connect(UBTDataCenter,&DataCenter::playMusic,this,&BehaviorLayerContainer::startPlay);
    connect(UBTDataCenter,&DataCenter::stopMusic,this,&BehaviorLayerContainer::stopPlay);
    connect(UBTDataCenter,&DataCenter::moveGroupsBehind,this,&BehaviorLayerContainer::onMoveGroupsBehind);
    connect(UBTDataCenter,&DataCenter::changeGroup,this,&BehaviorLayerContainer::onChangeGroup);
    connect(UBTDataCenter,&DataCenter::createGroup,this,&BehaviorLayerContainer::onCreateGroup);
    connect(UBTDataCenter,&DataCenter::groupDeleted,this,&BehaviorLayerContainer::onDeleteGroup);

    connect(UBTDataCenter,&DataCenter::framesSelectionChanged,this,&BehaviorLayerContainer::onFramesSelectionChanged);
    connect(UBTDataCenter,&DataCenter::groupsSelectionChanged,this,&BehaviorLayerContainer::onGroupsSelectionChanged);
    connect(UBTDataCenter,&DataCenter::clearMusicSelection,this,&BehaviorLayerContainer::onClearMusicSelection);
    connect(UBTDataCenter,&DataCenter::revertGroups,this,&BehaviorLayerContainer::onRevertGroups);
    connect(UBTDataCenter,&DataCenter::timeLineChanged,this,&BehaviorLayerContainer::onTimeLineChanged);
}

BehaviorLayerContainer::~BehaviorLayerContainer()
{
}

bool BehaviorLayerContainer::onNewBehavior()
{
    deleteMusic();
    for(auto widget : actionGroups_) {
        widget->deleteLater();
    }
    actionGroups_.clear();

    auto groups = UBTDataCenter->getGroups();

    for(auto cit = groups.cbegin(); cit != groups.cend(); ++cit) {
        auto groupName = cit.key();
        auto pos = groupName.indexOf('#');
        groupName = groupName.mid(pos+1);
        insertGroupWidget(groupName,cit.value());
    }

    return true;
}

void BehaviorLayerContainer::onFrameNumberChanged(int fn)
{
    frameNumber_ = fn;
    auto frame_distance = frameDistance();
    for (LayerBgWidget * layerBgWidget : layerBgWidgets_) {
        layerBgWidget->setFixedWidth(frame_distance * fn);
    }
    setFixedWidth(frame_distance * fn);
    updateContent();
}

void BehaviorLayerContainer::setScaleFactor(double sf)
{
    scaleFactor_ = sf;
    auto total_len = frameNumber_ * frameDistance();
    setFixedWidth(total_len);
    for (auto &w : layerBgWidgets_) {
        w->setFixedWidth(total_len);
    }
    ActionGroupWidget::frameDistance_ = frameDistance();
    QMap<int,QPair<QString,QSet<int>>> groups;
    for(auto groupWidget : actionGroups_) {
        auto groupName = groupWidget->groupName();
        auto frames = groupWidget->frameList().toSet();
        QPair<QString,QSet<int>> info;
        info.first = groupName;
        info.second = frames;
        groups.insert(groupWidget->groupIndex(),info);
        groupWidget->hide();
        groupWidget->deleteLater();
    }
    actionGroups_.clear();
    onGroupChanged(groups,DataCenter::kInvalidFrameIndex);
    if(musicWidget_) {
        plotAudioItem(musicWidget_->getAudioItem(),frameDistance());
    }
    updateContent();
}

void BehaviorLayerContainer::init()
{
    setObjectName("behaviorLayerContainer");
    setStyleSheet("QWidget#behaviorLayerContainer{ background: #252A35; }");
    ActionGroupWidget::frameDistance_ = frameDistance();

    mediaPlayer_ = new QMediaPlayer(this);
    connect(mediaPlayer_,&QMediaPlayer::mediaStatusChanged,[this](QMediaPlayer::MediaStatus status){
        qDebug() << "media state:" << status;
        if(status == QMediaPlayer::EndOfMedia) {
            UBTDataCenter->onMusicEnd();
        }
    });
    connect(mediaPlayer_,&QMediaPlayer::stateChanged,this,&BehaviorLayerContainer::stateChanged);
    setAcceptDrops(true);
//    setMouseTracking(true);
    setFixedWidth(frameNumber_ * frameDistance());
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(1);
    mainLayout->setMargin(0);
    for (int i = 0; i < layerCount_; ++i) {
        auto layerWidget = new LayerBgWidget(i,"test",this);
        layerBgWidgets_.push_back(layerWidget);
        layerBgWidgets_[i]->setMouseTracking(true);
        layerBgWidgets_[i]->setFixedHeight(LAYER_WIDGET_HEIGHT);
        mainLayout->addWidget(layerBgWidgets_[i]);
    }
    distanceWidget_ = new DistanceWidget(layerBgWidgets_[0]);
    distanceWidget_->hide();

    mainMenu_ = new QMenu(this);
    mainMenu_->setObjectName("behaviorMenu");
    copyAction_ = new QAction(tr("Copy"),this);
    copyAction_->setEnabled(false);
    pasteAction_ = new QAction(tr("Paste"),this);
    deleteAction_ = new QAction(tr("Delete"),this);
    deleteAction_->setEnabled(false);
    mainMenu_->addAction(copyAction_);
    mainMenu_->addAction(pasteAction_);
    mainMenu_->addAction(deleteAction_);
    auto action = new QAction(tr("rename"),mainMenu_);
    action->setEnabled(false);
    mainMenu_->addAction(action);
    action = new QAction(tr("save as template"),mainMenu_);
    action->setEnabled(false);
    mainMenu_->addAction(action);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(onRightMenu(const QPoint&)));

    connect(copyAction_,SIGNAL(triggered(bool)),this,SLOT(onCopy()));
    connect(pasteAction_,SIGNAL(triggered(bool)),this,SLOT(onPaste()));
    connect(deleteAction_,SIGNAL(triggered(bool)),this,SLOT(onDelete()));
}

void BehaviorLayerContainer::updateContent()
{
    update();
}

bool BehaviorLayerContainer::loadAudioData()
{
    deleteMusic();
    QString audioPath = DataCenter::getInstance()->getMusicFilePath();
    if(!audioPath.isEmpty()) {
        addMusic(audioPath);
    }
    return true;
}

void BehaviorLayerContainer::mousePressEvent(QMouseEvent* e)
{
    mouseMoved_ = false;
    ctrlMotion_ = false;
    mousePressed_ = true;
    bool ctrl = e->modifiers().testFlag(Qt::ControlModifier);
    auto pos = e->pos();
    pressPos_ = pos;
    pressedGroupWidget_ = groupWidgetAt(pos);
    if(pressedGroupWidget_ != nullptr && pos.y() <= kGroupTitleHeight) {
        UBTDataCenter->groupPressed(pressedGroupWidget_->groupIndex(),ctrl);
    } else if(pressedGroupWidget_ == nullptr) {
        drawSelectionRect_ = true;
        pos1 = pos2 = pos;
        layerBgWidgets_[0]->setDrawRect(true,pos);
    }
}

void BehaviorLayerContainer::mouseMoveEvent(QMouseEvent* event)
{
    bool ctrl = event->modifiers().testFlag(Qt::ControlModifier);
    auto currentPos = event->pos();
    bool validMotion = (qAbs(currentPos.x() - pressPos_.x()) > 5 || qAbs(currentPos.y() - pressPos_.y()) > 5);
    if(!mouseMoved_ && validMotion) {
        mouseMoved_ = true;
        if(pressedGroupWidget_ != nullptr) {
            int delta = currentPos.x() - pressPos_.x();
            if(ctrl) {
                ctrlMotion_ = true;
                UBTDataCenter->copySelectedGroupsData();
                for(auto cit = selectedGroupWidgets_.cbegin(); cit != selectedGroupWidgets_.cend(); ++cit) {
                    auto groupWidget = cit.value();
                    int groupIndex = cit.key();
                    ActionGroupWidget *newGroupWidget = new ActionGroupWidget(groupWidget->groupName(),
                                                                              groupWidget->frameList().toSet(),
                                                                              layerBgWidgets_[0]);
                    initGroupWidgetConnections(newGroupWidget);
                    newGroupWidget->show();
                    tempCreatedWidgets_.insert(groupIndex,newGroupWidget);
                    newGroupWidget->move(groupIndex*frameDistance()-kFrameWidgetWidth/2+delta,0);
                }
            } else {
                if(selectedGroupWidgets_.size() == 1) {
                    auto groupWidget = selectedGroupWidgets_.first();
                    int leftSide = calcLeftBound(groupWidget);
                    int newPos = groupWidget->geometry().left()+delta;
                    if(newPos < leftSide) {
                        newPos = leftSide;
                    }
                    int newDelta = newPos - groupWidget->geometry().left();
                    moveGroupsBehind(groupWidget->groupIndex(),newDelta);
                    groupWidget->move(newPos,0);
                }
            }
        }
    } else if(mouseMoved_) {
        if(pressedGroupWidget_ != nullptr) {
            int delta = currentPos.x() - lastPos_.x();
            if(ctrlMotion_) {
                for(auto cit = tempCreatedWidgets_.cbegin(); cit != tempCreatedWidgets_.cend(); ++cit) {
                    auto groupWidget = cit.value();
                    groupWidget->move(groupWidget->geometry().left()+delta,0);
                }
            } else {
                if(selectedGroupWidgets_.size() == 1) {
                    auto groupWidget = selectedGroupWidgets_.first();
                    int leftSide = calcLeftBound(groupWidget);
                    int newPos = groupWidget->geometry().left()+delta;
                    if(newPos < leftSide) {
                        newPos = leftSide;
                    }
                    int newDelta = newPos - groupWidget->geometry().left();
                    moveGroupsBehind(groupWidget->groupIndex(),newDelta);
                    groupWidget->move(newPos,0);
                    showDistanceWidget();
                }
            }
        }
    }
    lastPos_= currentPos;
    if(drawSelectionRect_) {
        pos2 = event->pos();
        boxChooseGroups();
    }
}

void BehaviorLayerContainer::mouseReleaseEvent(QMouseEvent* e)
{
    if(mousePressed_) {
        mousePressed_ = false;
    } else {
        return;
    }
    int frameIndex = (e->pos().x() + frameDistance()/2)/frameDistance();
    UBTDataCenter->setCurrentFrameIndex(frameIndex);
    bool ctrl = e->modifiers().testFlag(Qt::ControlModifier);
    if(drawSelectionRect_) {
        drawSelectionRect_ = false;
        layerBgWidgets_[0]->setDrawRect(false,e->pos());
    }
    if(mouseMoved_) {
        mouseMoved_ = false;
        distanceWidget_->setVisible(false);
        if(ctrlMotion_) {
            ctrlMotion_ = false;
            auto groupWidget = tempCreatedWidgets_.first();
            int newIndex = (groupWidget->geometry().left()+frameDistance()/2)/frameDistance();
            for(auto tempWidget : tempCreatedWidgets_) {
                tempWidget->hide();
                tempWidget->deleteLater();
            }
            tempCreatedWidgets_.clear();
            UBTDataCenter->setCurrentFrameIndex(newIndex);
            UBTDataCenter->pasetCopiedGroups();
        } else {
            if(pressedGroupWidget_ != nullptr) {
                auto groupWidget = selectedGroupWidgets_.first();
                int newIndex = (groupWidget->geometry().left()+frameDistance()/2)/frameDistance();
                UBTDataCenter->moveGroup(groupWidget->groupIndex(),newIndex);
            }
        }
    } else {
        if(pressedGroupWidget_ == nullptr) {
            if(!ctrl  && e->pos().y() <= kLayerHeight) { //避免点击music时反选，TODO待完善
                UBTDataCenter->clearSelection();
            }
        } else {
            if(pressPos_.y() <= kGroupTitleHeight && e->button() == Qt::LeftButton) {
                UBTDataCenter->groupReleased(pressedGroupWidget_->groupIndex(),ctrl);
            }
        }
    }
    pressedGroupWidget_ = nullptr;
}

void BehaviorLayerContainer::setFrameRate(int fps)
{
    if(musicWidget_) {
        plotAudioItem(musicWidget_->getAudioItem(),frameDistance());
    }
}

void BehaviorLayerContainer::onRightMenu(const QPoint &pos)
{
    auto copyType = UBTDataCenter->getCopyType();
    if(layerBgWidgets_[0]->geometry().contains(pos)) {
        switch(copyType) {
        case DataCenter::kCopyNone:
            pasteAction_->setEnabled(false);
            break;
        case DataCenter::kCopyFrame:
        case DataCenter::kCopyGroup:
            pasteAction_->setEnabled(true);
            break;
        }
        mainMenu_->move(QCursor::pos());
        mainMenu_->show();
    }

}

void BehaviorLayerContainer::plotAudioItem(AudioItem *item, double frameDis)
{
    if (!item) {
        return;
    }
    auto width = static_cast<double>(item->getPlayTime()) / DataCenter::getInstance()->frameDivision() * frameDis;
    musicWidget_->setFixedWidth(width);
    item->setFixedWidth(width);
//    item->move(0, 1);
    item->storeStartDragPos();
    item->updateAudioPaintSamples();
    item->setVisible(true);
    int musicEndIndex = item->getPlayTime() / DataCenter::getInstance()->frameDivision();
    if(item->getPlayTime() % DataCenter::getInstance()->frameDivision() != 0) {
        ++musicEndIndex;
    }
    UBTDataCenter->setMusicEndFrameIndex(musicEndIndex);
}

int BehaviorLayerContainer::getMusicDuration(const QString &filePath) const
{
    AudioInfo audioInfo;
    if (-1 != Audio::getAudioInfo(filePath, audioInfo)) {
        return audioInfo.duration;
    } else {
        return 0;
    }
}

void BehaviorLayerContainer::deleteMusic()
{
    if(musicWidget_) {
        musicWidget_->setVisible(false);
        musicWidget_->deleteLater();
        musicWidget_ = nullptr;
        UBTDataCenter->deleteMusic();
    }
}


void BehaviorLayerContainer::playAudioAtFrame(int frame)
{
    if(!musicWidget_) {
        return;
    }
    auto audioItem = musicWidget_->getAudioItem();
    auto itemStopFrame = qRound(static_cast<double>(audioItem->getPlayTime()) / DataCenter::getInstance()->frameDivision());
    if (frame < itemStopFrame) {
        auto startPlayTime = DataCenter::getInstance()->frameDivision() * frame;
        mediaPlayer_->setMedia(QUrl::fromLocalFile(audioItem->getFilePath()));
        mediaPlayer_->setPosition(startPlayTime);
        mediaPlayer_->play();
    }
}

void BehaviorLayerContainer::onCopy()
{
    auto selectionType = UBTDataCenter->getSelectionType();
    switch (selectionType) {
    case DataCenter::kSelectionGroup:
    {
        UBTDataCenter->copySelectedGroupsData();
    }
        break;
    case DataCenter::kSelectionFrame:
    {
        UBTDataCenter->copyFramesData();
    }
        break;
    default:
        break;
    }
}

void BehaviorLayerContainer::onPaste()
{
    auto copyType = UBTDataCenter->getCopyType();
    switch (copyType) {
    case DataCenter::kCopyGroup:
        UBTDataCenter->pasetCopiedGroups();
        break;
    case DataCenter::kCopyFrame:
        UBTDataCenter->pasteCopiedFrames();
        break;
    default:
        break;
    }
}

void BehaviorLayerContainer::onDelete()
{
    auto selectionType = UBTDataCenter->getSelectionType();
    switch (selectionType) {
    case DataCenter::kSelectionMusic:
        deleteMusic();
        break;
    case DataCenter::kSelectionGroup:
    {
        UBTDataCenter->deleteSelectedGroups();
    }
        break;
    case DataCenter::kSelectionFrame:
        UBTDataCenter->deleteSelectedFrames();
        break;
    default:
        break;
    }
}
void BehaviorLayerContainer::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Delete) {
        onDelete();
        return;
    }
    if (e->modifiers() == Qt::ControlModifier) {
        switch (e->key()) {
        case Qt::Key_C: {
            onCopy();
        }
            break;
        case Qt::Key_V: {
            onPaste();
        }
            break;
        case Qt::Key_Y: {
            UBTDataCenter->redoChanges();
        }
            break;
        case Qt::Key_Z: {
            UBTDataCenter->undoChanges();
        }
            break;
//        case Qt::Key_A: {
//            UBTDataCenter->selectAllGroups();
//        }
//            break;
        default:
            break;
        }
    }
}

void BehaviorLayerContainer::onKeyEvent(QKeyEvent *e)
{
    keyPressEvent(e);
}

double BehaviorLayerContainer::frameDistance() const
{
    return frameDistanceBase_ * scaleFactor_;
}

void BehaviorLayerContainer::startPlay(int startFrame)
{
    playAudioAtFrame(startFrame);
}

void BehaviorLayerContainer::stopPlay()
{
    if (mediaPlayer_) {
        mediaPlayer_->stop();
    }
}

void BehaviorLayerContainer::addMusic(const QString& filePath)
{
    if(filePath.isEmpty()) {
        return;
    }
    if(musicWidget_) {
        musicWidget_->setVisible(false);
        musicWidget_->deleteLater();
    }
    musicWidget_ = new MusicWidget(filePath,layerBgWidgets_[audioLayerId_]);
    connect(musicWidget_,&MusicWidget::deleteMusic,this,&BehaviorLayerContainer::onMusiceDelete);
    connect(musicWidget_,&MusicWidget::musicSelected,this,&BehaviorLayerContainer::onMusicSelected);
    musicWidget_->setSelected(true);
    musicWidget_->move(0,0);
    musicWidget_->show();
    plotAudioItem(musicWidget_->getAudioItem(),frameDistance());
}

void BehaviorLayerContainer::updateGroupWidgetPrevious()
{
    ActionGroupWidget* lastWidget = nullptr;
    for(auto widget : actionGroups_) {
        widget->setPreviousWidget(lastWidget);
        lastWidget = widget;
    }
}

ActionGroupWidget *BehaviorLayerContainer::insertGroupWidget(QString groupName, QSet<int> frames)
{
    ActionGroupWidget *newGroupWidget = new ActionGroupWidget(groupName,
                                                              frames,
                                                              layerBgWidgets_[0]);
    initGroupWidgetConnections(newGroupWidget);
    newGroupWidget->show();
    actionGroups_.insert(newGroupWidget->groupIndex(),newGroupWidget);
    updateGroupWidgetPrevious();
    return newGroupWidget;
}

void BehaviorLayerContainer::initGroupWidgetConnections(ActionGroupWidget *groupWidget)
{
    connect(groupWidget,&ActionGroupWidget::groupWidgetMoved,this,&BehaviorLayerContainer::moveGroupsBehind);
    connect(groupWidget,&ActionGroupWidget::groupMoved,this,&BehaviorLayerContainer::onGroupMoved);
    connect(groupWidget,&ActionGroupWidget::groupReleased,this,&BehaviorLayerContainer::onGroupReleased);
    connect(groupWidget,&ActionGroupWidget::groupPressed,this,&BehaviorLayerContainer::onGroupPressed);
    connect(groupWidget,&ActionGroupWidget::framePressed,this,&BehaviorLayerContainer::onFramePressed);
    connect(groupWidget,&ActionGroupWidget::combineGroups,this,&BehaviorLayerContainer::onCombineGroups);
}

void BehaviorLayerContainer::addFileToFrame(int frameIndex, const QString &filePath)
{
    UBTDataCenter->insertTemplateFileData(frameIndex,filePath);
}

void BehaviorLayerContainer::initConfiguration()
{
    fullLayerId_ = 0;
    audioLayerId_ = 1;
    layerCount_ = 2;
}

ActionGroupWidget *BehaviorLayerContainer::getPreviousGroupWidget(int frameIndex)
{
    ActionGroupWidget *previousWidget = nullptr;
    for(auto cit = actionGroups_.cbegin(); cit != actionGroups_.cend(); ++cit) {
        if(cit.key() < frameIndex && !cit.value()->containsFrameIndex(frameIndex)) {
            previousWidget = cit.value();
        } else {
            break;
        }
    }
    return previousWidget;
}

void BehaviorLayerContainer::onGroupChanged(QMap<int,QPair<QString,QSet<int>>> groups,int selectFrameIndex)
{
    for(auto groupWidget : actionGroups_) {
        groupWidget->hide();
        groupWidget->deleteLater();
    }
    actionGroups_.clear();
    for(auto cit = groups.cbegin(); cit != groups.cend(); ++cit) {
        auto info = cit.value();
        insertGroupWidget(info.first,info.second);
    }
    if(selectFrameIndex != DataCenter::kInvalidFrameIndex) {
        selectFrameWidget(selectFrameIndex);
    }
}

void BehaviorLayerContainer::selectFrameWidget(int frameIndex)
{
    for(auto groupWidget : actionGroups_) {
        if(groupWidget->frameList().contains(frameIndex)) {
            groupWidget->selectFrameWidget(frameIndex);
            break;
        }
    }
}

void BehaviorLayerContainer::moveGroupsBehind(int groupId, int delta)
{
    for(auto cit = actionGroups_.cbegin(); cit != actionGroups_.cend(); ++cit) {
        if(cit.key() > groupId) {
            auto widget = cit.value();
            widget->move(widget->geometry().left()+delta,0);
        }
    }
}

void BehaviorLayerContainer::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QWidget::paintEvent(event);
}

void BehaviorLayerContainer::wheelEvent(QWheelEvent* event)
{
    if ( QApplication::keyboardModifiers () == Qt::ControlModifier)
    {
        qDebug() << "ctrl wheel event " << event->delta();
        if (event->delta() > 0) {
            emit zoomIn();
        } else {
            emit zoomOut();
        }
    }
    event->accept();
}

void BehaviorLayerContainer::onMusiceDelete(const QString& musicName)
{
    deleteMusic();
}

void BehaviorLayerContainer::showDistanceWidget()
{
    if(selectedGroupWidgets_.size() != 1) {
        return;
    }

    auto firstWidget = selectedGroupWidgets_.first();

    auto previousWidget = firstWidget->previousWidget();
    if(previousWidget != nullptr) {
        distanceWidget_->move(previousWidget->geometry().right(),48);
        int distance = firstWidget->geometry().left() - previousWidget->geometry().right();
        distanceWidget_->setFixedWidth(distance);
        distanceWidget_->setDistance(QString::number((distance+kFrameWidgetWidth)/(int)frameDistance()));
        if(!distanceWidget_->isVisible()) {
            distanceWidget_->setVisible(true);
        }
    } else {
        distanceWidget_->setVisible(false);
    }
}

void BehaviorLayerContainer::onGroupPressed(int groupIndex, bool ctrl)
{
    if(!ctrl) {
        for(auto tempGroup : selectedGroups_) {
            tempGroup->clearSelection();
            selectedGroups_.remove(tempGroup);
        }
    }
    selectedGroups_.insert(actionGroups_[groupIndex]);
    selectionType_ = kSelectionGroups;
    if(musicWidget_) {
        musicWidget_->setSelected(false);
    }
}

void BehaviorLayerContainer::onFramePressed(int groupIndex, int frameIndex, bool ctrl)
{
    for(auto cit = actionGroups_.cbegin(); cit != actionGroups_.cend(); ++cit) {
        auto widget = cit.value();
        if(!widget->containsFrameIndex(frameIndex)) {
            widget->clearSelection();
        }
    }
    selectionType_ = kSelectionFrames;
    selectedFrameIndex_ = frameIndex;
    if(musicWidget_) {
        musicWidget_->setSelected(false);
    }
}

void BehaviorLayerContainer::onGroupReleased()
{
    distanceWidget_->setVisible(false);
}

void BehaviorLayerContainer::onGroupMoved()
{
    showDistanceWidget();
}

void BehaviorLayerContainer::onMoveGroupsBehind(QSet<int> groups, int delta)
{
    QMap<int,ActionGroupWidget*> tempGroups;
    for(auto cit = actionGroups_.cbegin(); cit != actionGroups_.cend(); ++cit) {
        auto groupIndex = cit.key();
        auto groupWidget = cit.value();
        if(groups.contains(groupIndex)) {
            int newIndex = groupIndex+delta;
            groupWidget->move(newIndex*frameDistance()-kFrameWidgetWidth/2,0);
            groupWidget->setGroupIndex(newIndex);
            tempGroups.insert(newIndex,groupWidget);
        } else {
            tempGroups.insert(groupIndex,groupWidget);
        }
    }
    actionGroups_.swap(tempGroups);
    updateGroupWidgetPrevious();
}
void BehaviorLayerContainer::onChangeGroup(int originGroupIndex, QSet<int> frameIndexes)
{
    auto group = actionGroups_[originGroupIndex];
    auto groupName = group->groupName();
    actionGroups_.remove(originGroupIndex);
    group->deleteLater();
    group = insertGroupWidget(groupName,frameIndexes);
//    if(selectFrame != DataCenter::kInvalidFrameIndex) {
//        group->selectFrameWidget(selectFrame);
//    }
}

void BehaviorLayerContainer::onCreateGroup(QString groupName, QSet<int> frameIndexes)
{
    insertGroupWidget(groupName,frameIndexes);
}

void BehaviorLayerContainer::onDeleteGroup(int groupIndex)
{
    auto group = actionGroups_[groupIndex];
    actionGroups_.remove(groupIndex);
    group->hide();
    group->deleteLater();
    updateGroupWidgetPrevious();
}

void BehaviorLayerContainer::onMusicSelected(const QString& musicName, bool selected)
{
    selectionType_ = kSelectionMusic;
    for(auto cit = actionGroups_.cbegin(); cit != actionGroups_.cend(); ++cit) {
        auto widget = cit.value();
        widget->clearSelection();
    }
}

void BehaviorLayerContainer::onDivisionChanged()
{
    if(musicWidget_) {
        plotAudioItem(musicWidget_->getAudioItem(),frameDistance());
    }
    for(auto widget : actionGroups_) {
        widget->deleteLater();
    }
    actionGroups_.clear();
    auto groups = UBTDataCenter->getGroups();
    for(auto cit = groups.cbegin(); cit != groups.cend(); ++cit) {
        auto groupName = cit.key();
        auto pos = groupName.indexOf('#');
        groupName = groupName.mid(pos+1);
        insertGroupWidget(groupName,cit.value());
    }
}

void BehaviorLayerContainer::onCombineGroups()
{
    QSet<int> groups;
    for(auto tempGroup : selectedGroups_) {
        groups.insert(tempGroup->groupIndex());
    }
    if(groups.size() >= 2) {
        selectedGroups_.clear();
        UBTDataCenter->combineSelectedGroups();
    }
}

ActionGroupWidget* BehaviorLayerContainer::groupWidgetAt(QPoint pos)
{
    ActionGroupWidget* group = nullptr;
    for(auto tempGroup : actionGroups_) {
        if(tempGroup->geometry().contains(pos)) {
            group = tempGroup;
            break;
        }
    }
    return group;
}

ActionGroupWidget* BehaviorLayerContainer::getGroupByFrame(int frameIndex)
{
    ActionGroupWidget* group = nullptr;
    for(auto tempGroup : actionGroups_) {
        if(tempGroup->containsFrameIndex(frameIndex)) {
            group = tempGroup;
            break;
        }
    }
    return group;
}

void BehaviorLayerContainer::onFramesSelectionChanged(QSet<int> frames)
{
    clearAllSelection();
    if(frames.empty()) {
        return;
    }
    int frameIndex = frames.toList().first();
    ActionGroupWidget::groupOfSelectedFrames_  = getGroupByFrame(frameIndex);
    for(auto tempFrameIndex : frames) {
        ActionGroupWidget::groupOfSelectedFrames_->selectFrameWidget(tempFrameIndex);
    }
}
void BehaviorLayerContainer::onGroupsSelectionChanged(QSet<int> groups)
{
    clearAllSelection();
    if(groups.empty()) {
        return;
    }
    for(auto tempGroupIndex : groups) {
        auto groupWidget = actionGroups_[tempGroupIndex];
        groupWidget->setGroupSelected(true);
        selectedGroupWidgets_.insert(tempGroupIndex,groupWidget);
    }
}

void BehaviorLayerContainer::onClearMusicSelection()
{
    if(musicWidget_) {
        musicWidget_->setSelected(false);
    }
}

void BehaviorLayerContainer::clearAllSelection()
{
    for(auto tempGroupWidget : actionGroups_) {
        tempGroupWidget->clearSelection();
    }
    selectedGroupWidgets_.clear();
    ActionGroupWidget::groupOfSelectedFrames_ = nullptr;
    ActionGroupWidget::selectedFrameWidgets_.clear();
    if(musicWidget_) {
        musicWidget_->setSelected(false);
    }
}

void BehaviorLayerContainer::onRevertGroups(QMap<QString,QSet<int>> fullGroups)
{
    for(auto widget : actionGroups_) {
        widget->deleteLater();
    }
    actionGroups_.clear();

    for(auto cit = fullGroups.cbegin(); cit != fullGroups.cend(); ++cit) {
        auto groupName = cit.key();
        auto pos = groupName.indexOf('#');
        groupName = groupName.mid(pos+1);
        insertGroupWidget(groupName,cit.value());
    }

}

void BehaviorLayerContainer::onTimeLineChanged()
{
    setFocus();
}

int BehaviorLayerContainer::calcLeftBound(ActionGroupWidget* groupWidget)
{
    int leftSide = frameDistance() - kFrameWidgetWidth/2;
    auto previous = groupWidget->previousWidget();
    if(previous != nullptr) {
        leftSide = previous->geometry().right() + frameDistance() - kFrameWidgetWidth;
    }
    return leftSide;
}

void BehaviorLayerContainer::boxChooseGroups()
{
    if(drawSelectionRect_) {
        QRect rect(pos1,pos2);
        QSet<int> groups;
        for(auto group : actionGroups_) {
            if(group->geometry().intersects(rect)) {
                groups.insert(group->groupIndex());
            }
        }
        UBTDataCenter->selectGroups(groups);
    }
}
