#include "datacenter.h"

#include <QTextCodec>
#include <QFileInfo>
#include <QTime>
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <QLineEdit>
#include <QLabel>
#include <QDir>
#include <QHBoxLayout>
#include <QProcess>
#include <algorithm>
#include <list>

#include "robotcfg.h"
#include "commondefine.h"
#include "yansheehts.h"
#include "commondialog.h"
#include "apollocommunication.h"
#include "robotsearchdialog.h"
#include "savefiledialog.h"
#include "behavior.h"
#include "toastdialog.h"
#include "classstudiosettings.h"
#include "nameinputwidget.h"
#include "yansheelayer.h"

const QString kServoIdFormatString("ID %1");
const QString kUBTUntitledFileName("ubt_untitled");
const QString kBehaviorFileSuffix(".xml");

DataCenter::DataCenter(QObject* parent)
    : QObject(parent)
    , currentFrameIndex_(1) //第0帧为起始姿态
    , isPlaying_(false)
    , yansheeHts_(nullptr)
    , apolloHandler_(new ApolloCommunication)
    , handMirrorEnabled_(false)
    , legMirrorEnabled_(false)
    , batteryPercent_(0)
    , batteryCharging_(0)
    , volume_(0)
    , addFrameInterval_(10)
    , fileEdited_(false)
    , previewIndex_(0)
    , keyFrameModified_(false)
    , behaviorData_(nullptr)
    , frameDivision_(200)
    , copyType_(kCopyNone)
    , playType_(kPlayNone)
    , musicEndFrameIndex_(0)
    , playingAction_(true)
    , playingMusic_(true)
    , selectionType_(kSelectionNone)
    , revertIndex_(0)
{
    yansheeLayer_ = new YansheeLayer(this);
    previewTimer_ = new QTimer(this);
    previewTimer_->setTimerType(Qt::PreciseTimer);
    connect(previewTimer_, &QTimer::timeout, this, &DataCenter::onPreviewTimerTimeout);
    playerTimer_ = new QTimer(this);
    playerTimer_->setTimerType(Qt::PreciseTimer);
    connect(playerTimer_, &QTimer::timeout, this, &DataCenter::onPlayerTimerTimeout);

//    connect(apolloHandler_,&ApolloCommunication::robotConnectionStateChanged,this,&DataCenter::robotConnectionStateChanged);
    connect(apolloHandler_,&ApolloCommunication::robotServosAngles,this,&DataCenter::onRobotServosAngles);
    connect(apolloHandler_,&ApolloCommunication::robotActionList,this,&DataCenter::robotActionList);
    connect(apolloHandler_,&ApolloCommunication::sendFileResult,this,&DataCenter::sendFileResult);
    connect(apolloHandler_,&ApolloCommunication::batteryState,[this](int inCharging, int percent){
            batteryCharging_ = inCharging;
            emit chargingStateNotify(inCharging);
            batteryPercent_ = percent;
            emit batteryPercentNotify(percent);
    });
    connect(apolloHandler_,&ApolloCommunication::volumeState,[this](int volume){
        volume_ = volume;
        emit volumeNotify(volume);
    });
    connect(apolloHandler_,&ApolloCommunication::robotConnectionStateChanged,this,&DataCenter::onConnectionChanged);
}

DataCenter* DataCenter::getInstance()
{
    static DataCenter dataCenter;
    return &dataCenter;
}

int DataCenter::frameDivision()
{
    return frameDivision_;
}

int DataCenter::frameRate()
{
    return 1000/frameDivision_;
}

std::string DataCenter::convertUtf8toString (const QString& filePath)
{
    QTextCodec* code_gbk = QTextCodec::codecForName("system");
    std::string str = code_gbk->fromUnicode(filePath).data();
    return str;
}

QString DataCenter::calcFormatTimeString(int startTime,bool withFrame)
{
    QString formatTime = QTime::fromMSecsSinceStartOfDay(startTime).toString("mm:ss");
    if(withFrame){
        int ms2 = startTime%1000;
        int remainFrame = ms2/frameDivision();
        formatTime += (":"+QString("%1").arg(remainFrame,2,10,QChar('0')));
    }
    return formatTime;
}

bool DataCenter::newBehaviorData()
{
    return loadBehaviorData("");
}

bool DataCenter::loadBehaviorData(const QString& filePath)
{
    //判断是否需要保存当前文件
    if(fileEdited_) {
        CommonDialog* dialog = new CommonDialog(tr("System Prompt"),CommonDialog::SaveButton);
        dialog->setDisplayText(tr("File edited, are you sure to save old file and then open new file"));
        connect(dialog,&CommonDialog::sigSave,[this](bool save){
            if(save) {
                saveBehaviorData();
            }
        });
        bool isCancel = false;
        connect(dialog,&CommonDialog::sigClose,[&isCancel]{
            isCancel = true;
        });
        dialog->exec();
        if(isCancel) {
            return false;
        }
    }
    //清除以前的数据
    clearSelection();
    if(behaviorData_ != nullptr) {
        delete behaviorData_;
    }
    behaviorData_ = new BEH::Behavior;
    fileEdited_ = false;
    fullFrames_.clear();
    for(auto tempGroup : fullGroups_) {
        tempGroup->deleteLater();
    }
    fullGroups_.clear();
    copyType_ = kCopyNone;
    copiedFramesData_.clear();
    setCurrentFrameIndex(1);
    isPlaying_ = false;
    playingAction_ = false;
    playingMusic_ = false;
    behaviorFilePath_ = filePath;
    musicFilePath_ = "";
    //新数据
    if(!filePath.isEmpty()) {
        QFileInfo fileInfo(filePath);
        if(fileInfo.suffix().compare("xml",Qt::CaseInsensitive) == 0) {
            behaviorName_ = fileInfo.baseName();
            if(behaviorData_->loadFile(convertUtf8toString(filePath).c_str())) {
                getDataFromBehavior();
                getLayerData();
                //QString layerFilePath = qApp->applicationDirPath() + "/temp/" + fileInfo.baseName() + ".layers";
                //yansheeLayer_->saveLayerFile(layerFilePath,fullHeadFrames_,fullHandFrames_,fullFootFrames_);
            }
        } else if(fileInfo.suffix().compare("hts",Qt::CaseInsensitive) == 0) {
            if(yansheeHts_->loadHtsFile(filePath,fullFrames_) && !fullFrames_.empty()) {
                int groupIndex = fullFrames_.firstKey();
                ActionGroup *group = new ActionGroup(fileInfo.baseName(),fullFrames_.keys().toSet(),this);
                fullGroups_.insert(groupIndex,group);
            }
            behaviorName_ = fileInfo.baseName();
        }
    } else {
        behaviorName_ = tr("Untitled");
    }
    emit newBehavior();
    onFramesChanged(false);
    emit fileOpend(behaviorName_);
    if(!musicFilePath_.isEmpty()) {
        emit sigAddMusic(musicFilePath_);
    }
    ClassStudioSettings settings;
    settings.setNoMoreTips(false);
    settings.setSetTimeTips(false);
    return true;
}

bool DataCenter::saveBehaviorData(bool saveAs)
{
    QFileInfo info(behaviorFilePath_);
    if (saveAs || behaviorFilePath_.isEmpty()) {
        QString filePath = getFileName();
        if(filePath.isEmpty()){
            return false;
        }
        if(!filePath.endsWith(kBehaviorFileSuffix)) {
            filePath += kBehaviorFileSuffix;
        }
        if(QFile::exists(filePath)) {
            QFileInfo newFileInfo(filePath);
            CommonDialog* dialog = new CommonDialog(tr("System Prompt"),CommonDialog::OnlyOkButton);
            dialog->setDisplayText(newFileInfo.baseName()+tr(" Already Existed."));
            dialog->setAbandonBtnText(tr("I Known"));
            dialog->show();
            return false;
        }
        behaviorFilePath_ = filePath;
    }
    if(behaviorFilePath_.isEmpty()) {
        return false;
    }
    QFileInfo fileInfo(behaviorFilePath_);
    QString baseName = fileInfo.baseName();
    behaviorName_ = baseName;
    QString filePath = fileInfo.absolutePath();
    bool ret =  saveBehaviorData(behaviorFilePath_);

    fileEdited_ = false;
    emit fileSaved(baseName);
    ToastDialog *toast = new ToastDialog();
    toast->show();
    return ret;
}

bool DataCenter::saveBehaviorData(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    htsFilePath_ = qApp->applicationDirPath() + "/temp/" + fileInfo.baseName() + ".hts";
    yansheeHts_->saveHtsFile(htsFilePath_,fullFrames_);
    if(behaviorData_) {
        delete behaviorData_;
    }
    behaviorData_ = new BEH::Behavior;
    behaviorData_->setFrameRate(frameRate());
    for(auto cit = fullFrames_.cbegin(); cit != fullFrames_.cend(); ++cit) {
        auto servos = cit.value();
        for(auto cit2 = servos.cbegin(); cit2 != servos.cend(); ++cit2) {
            auto servo = std::make_shared<BEH::Servo>(cit2.value());
            behaviorData_->addServo(cit.key(),cit2.key(),servo);
        }
    }
    QMap<QString,std::set<int>> groups;
    for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
        groups.insert(cit.value()->groupName(),cit.value()->frameIndexesStdSet());
    }
    for(auto cit = groups.cbegin(); cit != groups.cend(); ++cit) {
        behaviorData_->queryActionLayer()->addGroup(cit.key().toUtf8().toStdString(),cit.value());
    }
    if(!musicFilePath_.isEmpty() && QFile::exists(musicFilePath_)) {
        QFileInfo info(filePath);
        QString dirPath = info.absolutePath();
        QFileInfo musicInfo(musicFilePath_);
        QString newPath = dirPath + "/" + info.baseName() + "." + musicInfo.suffix();
        if(newPath != musicFilePath_) {
            if(QFile::exists(newPath)) {
                bool ret = QFile::remove(newPath);
                qDebug() << "remove" << newPath << ret;
            }
            bool ret = QFile::copy(musicFilePath_,newPath);
            qDebug() << "copy2" << ret;
            musicFilePath_ = newPath;
        }

        QFileInfo newMusicInfo(musicFilePath_);
        auto musicItem = std::make_shared<BEH::MusicItem>();
        musicItem->setPath(newMusicInfo.fileName().toUtf8().toStdString());
        behaviorData_->queryMusicLayer()->addMusicItem(0,musicItem);
    }

    return behaviorData_->saveFile(convertUtf8toString(filePath).c_str());
}

void DataCenter::saveDataToTemplate(int groupIndex)
{
    if(!fullGroups_.contains(groupIndex)) {
        return;
    }
    auto groupName = fullGroups_[groupIndex]->groupName();
    CommonDialog *dialog = new CommonDialog(tr("Save As Template"));
    dialog->setCloseOnOk(false);
    NameInputWidget* widget = new NameInputWidget(tr("template name:"),groupName,dialog);
    dialog->setDisplayWidget(widget);
    connect(dialog,&CommonDialog::sigAccepted,[this,widget,dialog,groupIndex](bool accepted){
        if(accepted) {
            auto templateName = widget->newName();
            QString dirPath = QApplication::applicationDirPath() +
                    QString::fromLocal8Bit("/resource/libraries/动作/自定义/") +
                    templateName;
            QDir dir;
            if(dir.exists(dirPath)) {
                widget->setWarningText(templateName+tr(" Already Existed."));
            } else {
                dialog->close();
                saveDataToTemplatePrivate(groupIndex,templateName);
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

void DataCenter::saveDataToTemplatePrivate(int groupIndex, QString templateName)
{
    auto group = fullGroups_[groupIndex];
    auto frameIndexes = group->frameIndexes();
    QMap<int,QMap<int,double>> framesData;
    for(int frameIndex : frameIndexes) {
       framesData.insert(frameIndex-groupIndex+1,fullFrames_[frameIndex]);
    }
    BEH::Behavior behavior;
    behavior.setFrameRate(frameRate());
    std::set<int> frameSet;
    for(auto cit = framesData.cbegin(); cit != framesData.cend(); ++cit) {
        auto servos = cit.value();
        for(auto cit2 = servos.cbegin(); cit2 != servos.cend(); ++cit2) {
            auto servo = std::make_shared<BEH::Servo>(cit2.value());
            behavior.addServo(cit.key(),cit2.key(),servo);
        }
        frameSet.insert(cit.key());
    }
    behavior.queryActionLayer()->addGroup(templateName.toUtf8().toStdString(),frameSet);

    QString dirPath = QApplication::applicationDirPath() + QString::fromLocal8Bit("/resource/libraries/动作/自定义/") + templateName;
    QDir dir;
    if(!dir.exists(dirPath)) {
        dir.mkdir(dirPath);
        QString filePath = dirPath + "/" + templateName + kBehaviorFileSuffix;
        behavior.saveFile(convertUtf8toString(filePath).c_str());
        emit addFileToTemplate(filePath);
    }
}

int DataCenter::getCurrentConnectState()
{
    return currentConnectState;
}

void DataCenter::deleteMusic()
{
    musicFilePath_.clear();
    musicEndFrameIndex_ = 0;
    emit playableStateChanged();
    setFileEdited();
}

QMap<int, QMap<int, double> > DataCenter::getDataFromBehaviorFile(const QString &filePath)
{
    QMap<int, QMap<int, double>> framesData;
    BEH::Behavior behaviorData;
    behaviorData.loadFile(convertUtf8toString(filePath).c_str());
    auto actionLayer = behaviorData.queryActionLayer();
    if (actionLayer == nullptr) {
        return framesData;
    }
    auto frames = actionLayer->getFrames();
    for (auto itr = frames->cbegin(); itr != frames->cend(); ++itr) {
        int frameIndex = itr->first;
        auto framePtr = itr->second;
        auto servos = framePtr->getServos();
        for (auto itr2 = servos->cbegin(); itr2 != servos->cend(); ++itr2) {
            auto servoId = itr2->first;
            std::shared_ptr<BEH::Servo> servo = itr2->second;
            double servoAngle = servo->getTargetAngle();
            framesData[frameIndex].insert(servoId,servoAngle);
        }
    }
    int fileFrameDivision = behaviorData.getDivision();
    if(fileFrameDivision != frameDivision_) {
        QMap<int, QMap<int, double>> tempFramesData;
        framesData.swap(tempFramesData);
        int lastFrameIndex = -1;
        for(auto cit = tempFramesData.cbegin(); cit != tempFramesData.cend(); ++cit) {
            int frameIndex = cit.key();
            int newIndex = frameIndex*fileFrameDivision/frameDivision_;
            if(lastFrameIndex != -1 && newIndex <= lastFrameIndex) {
                newIndex = lastFrameIndex+1;
            }
            lastFrameIndex = newIndex;
            framesData.insert(newIndex,cit.value());
        }
    }
    return std::move(framesData);
}

void DataCenter::deleteSelectedFrames()
{
    if(selectedFrames_.empty()) {
        return;
    }
    int oneFrame = selectedFrames_.toList().first();
    auto group = getGroupByFrameIndex(oneFrame);
    if(group == nullptr) {
        return;
    }
    int groupIndex = group->groupIndex();
    if(group->frameCount() == selectedFrames_.size()) {
        auto frameIndexes = group->frameIndexes();
        for(auto frameIndex : frameIndexes) {
            fullFrames_.remove(frameIndex);
        }
        fullGroups_.remove(groupIndex);
        group->deleteLater();

        emit groupDeleted(groupIndex);
        onFramesChanged(true);
        clearSelection();
        return;
    }

    for(auto frameIndex : selectedFrames_) {
        fullFrames_.remove(frameIndex);
        group->removeIndex(frameIndex);
    }
    int newGroupIndex = group->groupIndex();
    if(newGroupIndex != groupIndex) {
        fullGroups_.remove(groupIndex);
        fullGroups_.insert(newGroupIndex,group);
    }
    emit changeGroup(groupIndex,group->frameIndexes());
    onFramesChanged(true);
    clearSelection();
}

void DataCenter::deleteSelectedGroups()
{
    for(auto groupIndex : selectedGroups_) {
        auto group = fullGroups_[groupIndex];
        fullGroups_.remove(groupIndex);
        auto frameIndexes = group->frameIndexes();
        for(auto frameIndex : frameIndexes) {
            fullFrames_.remove(frameIndex);
        }
        group->deleteLater();
        emit groupDeleted(groupIndex);
    }
    onFramesChanged(true);
    clearSelection();
}

void DataCenter::setCurrentFrameIndex(int index)
{
    if(index <= 0) {
        index = 1;
    }
    if(currentFrameIndex_ != index) {
        currentFrameIndex_ = index;
        emit currentFrameIndexChanged(currentFrameIndex_);
        emit keyFrameModified(false);
    }
}

void DataCenter::setFrameDivision(int frameDivision)
{
    if(frameDivision != frameDivision_) {
        int lastFrameDivision = frameDivision_;
        frameDivision_ = frameDivision;
        yansheeHts_->setDivisionCount(frameDivision/yansheeHts_->unitDivision());
        yansheeLayer_->setFrameDivision(frameDivision_);
        behaviorData_->setFrameRate(frameRate());
        if(!fullFrames_.empty()) {
            QMap<int,int> frameMap;
            QMap<int,QMap<int,double>> tempFullFrames;
            int lastFrameIndex = 0;
            for(auto cit = fullFrames_.cbegin(); cit != fullFrames_.cend(); ++cit) {
                int frameIndex = cit.key();
                int newFrameIndex = (frameIndex*lastFrameDivision)/ frameDivision_;
                if(newFrameIndex <= lastFrameIndex) {
                    newFrameIndex = lastFrameIndex + 1;
                }
                lastFrameIndex = newFrameIndex;
                tempFullFrames.insert(newFrameIndex,cit.value());
                frameMap.insert(frameIndex,newFrameIndex);
            }
            fullFrames_.swap(tempFullFrames);
            QMap<int,ActionGroup*> tempFullGroups;
            for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
                auto group = cit.value();
                auto frameIndexes = group->frameIndexes();
                QSet<int> newFrameIndexes;
                for(auto frameIndex : frameIndexes) {
                    newFrameIndexes.insert(frameMap[frameIndex]);
                }
                group->changeIndexes(newFrameIndexes);
                tempFullGroups.insert(group->groupIndex(),group);
            }
            fullGroups_.swap(tempFullGroups);
            if(!fullFrames_.empty()) {
                endFrameIndex_ = fullFrames_.lastKey();
            } else {
                endFrameIndex_ = 0;
            }
            emit totalTimeChanged(endFrameIndex_*frameDivision_);
            setFileEdited();
        }
        emit frameDivisionChanged(frameDivision_);
    }
}

bool DataCenter::getNextFrameDataAndInterval(int frameIndex, QMap<int,double>& data, int& interval)
{
    auto iter = fullFrames_.upperBound(frameIndex);
    if(iter != fullFrames_.end()) {
        data = iter.value();
        interval = (iter.key() - frameIndex) * frameDivision();
        return true;
    }
    return false;
}

void DataCenter::setDeviceVolume(int volume)
{
    apolloHandler_->putDevicesVolume(volume);
}
void DataCenter::changeCurrentFrameServoNotToRobot(int servoId, double servoAngle)
{
    currentServosData_[servoId] = servoAngle;
    emit currentFrameServoChanged(servoId,servoAngle);
    if(fullFrames_.contains(currentFrameIndex_)) {
        emit keyFrameModified(true);
    }
}
void DataCenter::changeCurrentFrameServo(int servoId, double servoAngle)
{
    changeCurrentFrameServoNotToRobot(servoId,servoAngle);
    putServoAnglesToRobot(currentServosData_,100);
}

void DataCenter::servoAngleChangeFinished(int servoId, double servoAngle)
{
    changeRobotServoAngle(servoId,servoAngle);

}

void DataCenter::addFrame()
{
    auto changedGroup = getGroupByFrameIndex(currentFrameIndex_);
    if(changedGroup == nullptr) {
        QMap<int,QMap<int,double>> framesData;
        framesData.insert(currentFrameIndex_,currentServosData_);
        insertGroupData(calcGroupName(tr("ActionGroup")),framesData,currentFrameIndex_);
        onFramesChanged(true);
        QSet<int> frames;
        frames.insert(currentFrameIndex_);
        selectFrames(frames);
        return;
    }
    int groupIndex = changedGroup->groupIndex();
    int destIndex = currentFrameIndex_;
    QSet<int> behindGroups;
    if(changedGroup->hasFrame(destIndex)) {
        QMap<int,QMap<int,double>> newFrames;
        for(auto cit = fullFrames_.cbegin(); cit != fullFrames_.cend(); ++cit) {
            int tempFrameIndex = cit.key();
            if(tempFrameIndex <= destIndex) {
                newFrames.insert(tempFrameIndex,cit.value());
            } else {
                int newFrameIndex = tempFrameIndex + addFrameInterval_;
                newFrames.insert(newFrameIndex,cit.value());
            }
        }
        destIndex += addFrameInterval_;
        newFrames.insert(destIndex,currentServosData_);
        fullFrames_.swap(newFrames);

        auto frameIndexes = changedGroup->frameIndexes();
        QSet<int> newIndexes;
        for(auto tempIndex : frameIndexes) {
            if(tempIndex <= currentFrameIndex_) {
                newIndexes.insert(tempIndex);
            } else {
                newIndexes.insert(tempIndex+addFrameInterval_);
            }
        }
        newIndexes.insert(destIndex);
        changedGroup->changeIndexes(newIndexes);

        QMap<int,ActionGroup*> tempGroups;
        for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
            int tempGroupIndex = cit.key();
            auto tempGroup = cit.value();
            if(tempGroupIndex > groupIndex) {
                behindGroups.insert(tempGroupIndex);
                int newIndex = tempGroupIndex+addFrameInterval_;
                tempGroup->setGroupIndex(newIndex);
                tempGroups.insert(newIndex,tempGroup);
            } else {
                tempGroups.insert(tempGroupIndex,tempGroup);
            }
        }
        fullGroups_.swap(tempGroups);
    } else {
        fullFrames_.insert(destIndex,currentServosData_);
        changedGroup->addIndex(currentFrameIndex_);
    }
    setCurrentFrameIndex(destIndex);
    onFramesChanged(true);
    if(!behindGroups.empty()) {
        emit moveGroupsBehind(behindGroups,addFrameInterval_);
    }
    emit changeGroup(groupIndex,changedGroup->frameIndexes());
    QSet<int> frames;
    frames.insert(currentFrameIndex_);
    selectFrames(frames);
}

void DataCenter::setFileEdited()
{
    if(!fileEdited_) {
        fileEdited_ = true;
        emit fileEdited();
    }
}

void DataCenter::insertFrame(ActionGroup* group, int destIndex, QMap<int,double> data)
{
    int originGroupIndex = group->groupIndex();
    int originLast = group->lastFrameIndex();
    QSet<int> behindGroups;
    int delta = 0;
    if(group->hasFrame(destIndex)) {
        delta = 1;
        QMap<int,QMap<int,double>> newFrames;
        for(auto cit = fullFrames_.cbegin(); cit != fullFrames_.cend(); ++cit) {
            int tempFrameIndex = cit.key();
            if(tempFrameIndex < destIndex) {
                newFrames.insert(tempFrameIndex,cit.value());
            } else {
                int newIndex = tempFrameIndex + delta;
                newFrames.insert(newIndex,cit.value());
            }
        }
        newFrames.insert(destIndex,data);
        fullFrames_.swap(newFrames);

        auto frameIndexes = group->frameIndexes();
        QSet<int> newIndexes;
        for(auto tempIndex : frameIndexes) {
            if(tempIndex < destIndex) {
                newIndexes.insert(tempIndex);
            } else {
                newIndexes.insert(tempIndex+delta);
            }
        }
        newIndexes.insert(destIndex);
        group->changeIndexes(newIndexes);

        QMap<int,ActionGroup*> tempGroups;
        int changedGroupIndex = group->groupIndex();
        for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
            int tempGroupIndex = cit.key();
            auto tempGroup = cit.value();
            if(tempGroupIndex > changedGroupIndex) {
                behindGroups.insert(tempGroupIndex);
                int newIndex = tempGroupIndex+delta;
                tempGroup->setGroupIndex(newIndex);
                tempGroups.insert(newIndex,tempGroup);
            } else {
                tempGroups.insert(tempGroupIndex,tempGroup);
            }
        }
        fullGroups_.swap(tempGroups);
    } else {
        group->addIndex(destIndex);
        int newLast = group->lastFrameIndex();
        if(newLast != originLast) {
            delta = newLast - originLast;
            QMap<int,QMap<int,double>> newFrames;
            for(auto cit = fullFrames_.cbegin(); cit != fullFrames_.cend(); ++cit) {
                int tempFrameIndex = cit.key();
                if(tempFrameIndex <= originLast) {
                    newFrames.insert(tempFrameIndex,cit.value());
                } else {
                    int newIndex = tempFrameIndex + delta;
                    newFrames.insert(newIndex,cit.value());
                }
            }
            newFrames.insert(destIndex,data);
            fullFrames_.swap(newFrames);

            QMap<int,ActionGroup*> tempGroups;
            for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
                int tempGroupIndex = cit.key();
                auto tempGroup = cit.value();
                if(tempGroupIndex == originGroupIndex) {
                    continue;
                }
                if(tempGroupIndex > originGroupIndex) {
                    behindGroups.insert(tempGroupIndex);
                    int newIndex = tempGroupIndex+delta;
                    tempGroup->setGroupIndex(newIndex);
                    tempGroups.insert(newIndex,tempGroup);
                } else {
                    tempGroups.insert(tempGroupIndex,tempGroup);
                }
            }
            tempGroups.insert(group->groupIndex(),group);
            fullGroups_.swap(tempGroups);
        } else {
            fullFrames_.insert(destIndex,data);
            fullGroups_.remove(originGroupIndex);
            fullGroups_.insert(group->groupIndex(),group);
        }
    }
    if(!behindGroups.empty()) {
        emit moveGroupsBehind(behindGroups,delta);
    }
    emit changeGroup(originGroupIndex,group->frameIndexes());
}

void DataCenter::pasteCopiedFrames()
{
    if(copiedFramesData_.empty()) {
        return;
    }
    auto changedGroup = getGroupByFrameIndex(currentFrameIndex_);
    if(changedGroup == nullptr) {
        insertGroupData(calcGroupName(tr("ActionGroup")),copiedFramesData_,currentFrameIndex_);
        onFramesChanged(true);
        QSet<int> newSelectedFrames;
        int firstIndex = copiedFramesData_.firstKey();
        for(auto cit = copiedFramesData_.cbegin(); cit != copiedFramesData_.cend(); ++cit) {
            int tempDestIndex = cit.key() - firstIndex + currentFrameIndex_;
            newSelectedFrames.insert(tempDestIndex);
        }
        selectFrames(newSelectedFrames);
        return;
    }

    QSet<int> newSelectedFrames;
    int firstIndex = copiedFramesData_.firstKey();
    for(auto cit = copiedFramesData_.cbegin(); cit != copiedFramesData_.cend(); ++cit) {
        int destIndex = cit.key() - firstIndex + currentFrameIndex_;
        insertFrame(changedGroup,destIndex,cit.value());
        newSelectedFrames.insert(destIndex);
    }
    onFramesChanged(true);
    selectFrames(newSelectedFrames);
}

void DataCenter::pasteCopiedFramesInGroup(int groupIndex, int destIndex)
{
    if(copiedFramesData_.empty() || !fullGroups_.contains(groupIndex)) {
        return;
    }
    auto group = fullGroups_[groupIndex];

    QSet<int> newSelectedFrames;
    int firstIndex = copiedFramesData_.firstKey();
    for(auto cit = copiedFramesData_.cbegin(); cit != copiedFramesData_.cend(); ++cit) {
        int tempDestIndex = cit.key() - firstIndex + destIndex;
        insertFrame(group,tempDestIndex,cit.value());
        newSelectedFrames.insert(tempDestIndex);
    }
    onFramesChanged(true);
    selectFrames(newSelectedFrames);
}

void DataCenter::copySelectedGroupsData()
{
    copyType_ = kCopyGroup;
    copiedGroupsData_.clear();
    for(auto tempGroupIndex : selectedGroups_) {
        QMap<int,QMap<int,double>> framesData;
        auto tempGroup = fullGroups_[tempGroupIndex];
        auto frameIndexes = tempGroup->frameIndexes();
        for(auto tempIndex : frameIndexes) {
            framesData.insert(tempIndex,fullFrames_[tempIndex]);
        }
        QPair<QString,QMap<int,QMap<int,double>>> groupPair;
        groupPair.first = tempGroup->groupName();
        groupPair.second = framesData;
        copiedGroupsData_.insert(tempGroup->groupIndex(),groupPair);
    }
}

void DataCenter::copyFramesData()
{
    copyType_ = kCopyFrame;
    copiedFramesData_.clear();
    for(auto frameIndex : selectedFrames_) {
        copiedFramesData_.insert(frameIndex,fullFrames_[frameIndex]);
    }
}

void DataCenter::insertTemplateFileData(int frameIndex, const QString &filePath)
{
    QFileInfo info(filePath);
    auto data = getDataFromBehaviorFile(filePath);
    if(!data.empty()) {
        insertGroupData(info.baseName(),data,frameIndex);
        onFramesChanged(true);
        QSet<int> groups;
        groups.insert(frameIndex);
        selectGroups(groups);
    }
}

void DataCenter::insertGroupData(QString groupName, QMap<int,QMap<int,double>> data, int targetIndex)
{
    if(data.empty()) {
        return;
    }
    int delta = data.lastKey() - data.firstKey() + 1;
    int beginIndex = targetIndex;
    auto group = getGroupByFrameIndex(targetIndex);
    if(group != nullptr) {
        beginIndex = group->groupIndex();
        delta += targetIndex - beginIndex;
    } else {
        auto nextGroupIndex = fullGroups_.upperBound(targetIndex).key();
        int currentSpace = nextGroupIndex - targetIndex;
        if(currentSpace < delta) {
            delta = delta - currentSpace;
        } else {
            delta = 0;
        }
    }

    QMap<int,QMap<int,double>> tempFramesData;
    for(auto cit = fullFrames_.cbegin(); cit != fullFrames_.cend(); ++cit) {
        if(cit.key() >= beginIndex) {
            tempFramesData.insert(cit.key()+delta,cit.value());
        } else {
            tempFramesData.insert(cit.key(),cit.value());
        }
    }
    fullFrames_.swap(tempFramesData);

    int firstIndex = data.firstKey();
    QSet<int> frameIndexes;
    for(auto cit = data.cbegin(); cit != data.cend(); ++cit) {
        int newIndex = cit.key() - firstIndex + targetIndex;
        frameIndexes.insert(newIndex);
        fullFrames_.insert(newIndex,cit.value());
    }

    QSet<int> groupsBehind;
    QMap<int,ActionGroup*> tempGroups;
    for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
        int tempGroupIndex = cit.key();
        if(tempGroupIndex >= beginIndex) {
            groupsBehind.insert(tempGroupIndex);
            auto tempGroup = cit.value();
            int newIndex = tempGroupIndex+delta;
            tempGroup->setGroupIndex(newIndex);
            tempGroups.insert(newIndex,tempGroup);
        } else {
            tempGroups.insert(cit.key(),cit.value());
        }
    }
    fullGroups_.swap(tempGroups);
    ActionGroup* newGroup = new ActionGroup(groupName,frameIndexes,this);
    fullGroups_.insert(newGroup->groupIndex(),newGroup);

    if(!groupsBehind.empty()) {
        emit moveGroupsBehind(groupsBehind,delta);
    }
    emit createGroup(groupName,frameIndexes);
}

void DataCenter::pasetCopiedGroups()
{
    QSet<int> newSelectedGroups;
    int firstIndex = copiedGroupsData_.firstKey();
    for(auto cit = copiedGroupsData_.cbegin(); cit != copiedGroupsData_.cend(); ++cit) {
        int newIndex = cit.key() - firstIndex + currentFrameIndex_;
        insertGroupData(cit.value().first,cit.value().second,newIndex);
        newSelectedGroups.insert(newIndex);
    }
    onFramesChanged(true);
    selectGroups(newSelectedGroups);
}

void DataCenter::moveFrames(QMap<int,int> movedFrames)
{
    int firstFrameIndex = movedFrames.firstKey();
    auto group = getGroupByFrameIndex(firstFrameIndex);
    if(group == nullptr) {
        return;
    }
    int originGroupIndex = group->groupIndex();
    int originLast = group->lastFrameIndex();
    auto groupFrameIndexes = group->frameIndexes();
    if(groupFrameIndexes.size() == movedFrames.size()) {
        moveGroup(group->groupIndex(),movedFrames.first());
        selectFrames(movedFrames.values().toSet());
        return;
    }

    QMap<int,QMap<int,double>> groupData;
    for(auto frameIndex : groupFrameIndexes) {
        groupData.insert(frameIndex,fullFrames_[frameIndex]);
        fullFrames_.remove(frameIndex);
    }
    fullGroups_.remove(originGroupIndex);

    auto currentGroupData = groupData;
    QMap<int,QMap<int,double>> tempMoveData;
    auto moveList = movedFrames.keys();
    for(auto tempIndex : moveList) {
        tempMoveData.insert(movedFrames[tempIndex],groupData[tempIndex]);
        currentGroupData.remove(tempIndex);
    }
    auto targetList = movedFrames.values();
    for(auto tempIndex : targetList) {
        auto originData = tempMoveData[tempIndex];
        if(currentGroupData.contains(tempIndex)) {
            QMap<int,QMap<int,double>> tempData;
            for(auto cit = currentGroupData.cbegin(); cit != currentGroupData.cend(); ++cit) {
                if(cit.key() >= tempIndex) {
                    tempData.insert(cit.key()+1,cit.value());
                } else {
                    tempData.insert(cit.key(),cit.value());
                }
            }
            tempData.insert(tempIndex,originData);
            currentGroupData.swap(tempData);
        } else {
            currentGroupData.insert(tempIndex,originData);
        }
    }

    int newLast = currentGroupData.lastKey();
    int groupMoveDelta = newLast - originLast;
    QSet<int> behindGroups;
    if(fullGroups_.upperBound(originGroupIndex) != fullGroups_.end()) {
        int firstBehindGroupIndex = fullGroups_.upperBound(originGroupIndex).key();
        QMap<int,QMap<int,double>> newFrames;
        for(auto cit = fullFrames_.cbegin(); cit != fullFrames_.cend(); ++cit) {
            int tempFrameIndex = cit.key();
            if(tempFrameIndex >= firstBehindGroupIndex) {
                newFrames.insert(tempFrameIndex + groupMoveDelta,cit.value());
            } else {
                newFrames.insert(tempFrameIndex,cit.value());
            }
        }
        fullFrames_.swap(newFrames);

        QMap<int,ActionGroup*> tempGroups;
        for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
            if(cit.key() >= firstBehindGroupIndex) {
                behindGroups.insert(cit.key());
                int newGroupIndex = cit.key() + groupMoveDelta;
                cit.value()->setGroupIndex(newGroupIndex);
                tempGroups.insert(newGroupIndex,cit.value());
            } else {
                tempGroups.insert(cit.key(),cit.value());
            }
        }
        fullGroups_.swap(tempGroups);

        emit moveGroupsBehind(behindGroups,groupMoveDelta);
    }

    for(auto cit = currentGroupData.cbegin(); cit != currentGroupData.cend(); ++cit) {
        fullFrames_.insert(cit.key(),cit.value());
    }

    auto newFrameIndexes = currentGroupData.keys().toSet();
    group->changeIndexes(newFrameIndexes);
    fullGroups_.insert(group->groupIndex(),group);
    emit changeGroup(originGroupIndex,newFrameIndexes);

    onFramesChanged(true);
    selectFrames(movedFrames.values().toSet());
}

void DataCenter::moveGroup(int srcIndex, int destIndex)
{
    int delta = destIndex - srcIndex;
    QMap<int,QMap<int,double>> tempFrames;
    for(auto cit = fullFrames_.cbegin(); cit != fullFrames_.cend(); ++cit) {
        int frameIndex = cit.key();
        if(frameIndex < srcIndex) {
            tempFrames.insert(frameIndex,cit.value());
        } else {
            tempFrames.insert(frameIndex+delta,cit.value());
        }
    }
    fullFrames_.swap(tempFrames);

    QMap<int,ActionGroup*> tempGroups;
    QSet<int> groupsBehind;
    for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
        int tempGroupIndex = cit.key();
        auto tempGroup = cit.value();
        if(tempGroupIndex < srcIndex) {
            tempGroups.insert(tempGroupIndex,tempGroup);
        } else {
            groupsBehind.insert(tempGroupIndex);
            tempGroup->setGroupIndex(tempGroupIndex+delta);
            tempGroups.insert(tempGroupIndex+delta,tempGroup);
        }
    }
    fullGroups_.swap(tempGroups);

    onFramesChanged(true);
    if(!groupsBehind.empty()) {
        emit moveGroupsBehind(groupsBehind,delta);
    }

    QSet<int> newSelectedGroups;
    newSelectedGroups.insert(destIndex);
    selectGroups(newSelectedGroups);
}

void DataCenter::onFramesChanged(bool fileEdited)
{
    emit playableStateChanged();
    if(!fullFrames_.empty()) {
        endFrameIndex_ = fullFrames_.lastKey();
    } else {
        endFrameIndex_ = 0;
    }
    emit totalTimeChanged(endFrameIndex_*frameDivision_);
    if(fileEdited) {
        setFileEdited();
    }

    if(revertIndex_ != 0) {
        for(int i = 0; i < revertIndex_; ++i) {
            backupFullFrames_.pop_front();
            backupFullGroups_.pop_front();
        }
    } else {
        if(backupFullFrames_.size() == 6) {
            backupFullFrames_.pop_back();
            backupFullGroups_.pop_back();
        }
    }
    backupFullFrames_.push_front(fullFrames_);
    backupFullGroups_.push_front(getGroups());
    revertIndex_ = 0;
    emit timeLineChanged();
    qDebug() << __func__ << "frames" << fullFrames_.keys();
    if(fullGroups_.size() > 0) {
        qDebug() << __func__ << "groups" << fullGroups_.firstKey() << fullGroups_.first()->frameIndexes();
    }
    checkFramesInvterval();
}

void DataCenter::init()
{
    QDir dir(QApplication::applicationDirPath());
    bool mkdirResult = dir.mkdir("temp"); //用于存放打包数据等中间
    servoCount_ = RobotConfig->getServoCount();
    currentServosData_ = RobotConfig->getServosInitAngle();

#ifdef YANSHEE1_5
    yansheeHts_ = new YansheeHts(YansheeHts::Yanshee1_5,this);
#elif defined(YANSHEE1_1)
    yansheeHts_ = new YansheeHts(YansheeHts::Yanshee1_1,this);
#endif
}

QString DataCenter::getFileName()
{
    QString filePath = "";
    SaveFileDialog *saveFileDialog = new SaveFileDialog(DataCenter::getInstance()->behaviorFilePath());

    connect(saveFileDialog,&SaveFileDialog::sigSaveFile,[&filePath](QString fileName, QString fileDir){
        filePath = fileDir + "/" + fileName;
    });
    saveFileDialog->exec();
    qDebug() << "filePath====" << filePath;
    return filePath;
}

void DataCenter::connectRobot()
{
    qDebug() << "connectionState===" << apolloHandler_->robotConnectionState();
//    if(apolloHandler_->robotConnectionState() == ApolloCommunication::RobotUnconnected) {
        RobotSearchDialog *searchDialog = new RobotSearchDialog(apolloHandler_->robotConnectionState());
        searchDialog->setApolloCommunication(apolloHandler_);
        searchDialog->setSn(this->mSn);
        connect(searchDialog,&RobotSearchDialog::connectStateChanged,[=](int state,QString sn){
            this->currentConnectState = state;
            this->mSn = sn;
            emit robotConnectionStateChanged(state,sn);
        });
        searchDialog->exec();
}

QString DataCenter::compressCurrentBehavior()
{
    if(behaviorFilePath_.endsWith(kUBTUntitledFileName+kBehaviorFileSuffix)) {
        return QString();
    }
    if(fileEdited_) {
        saveBehaviorData();
    }
    QString zipFile = QString("%1/temp/%2.zip").arg(QApplication::applicationDirPath()).arg(behaviorName_);
    QFile file(zipFile);
    if(file.exists(zipFile)) {
        qDebug() << zipFile << "exists";
        bool ret = file.remove();
        qDebug() << "remove result" << ret;
    }

    QString filePath;
    if(!musicFilePath_.isEmpty()) {
//        QString cmdStr = QString("7z.exe a temp\\%1.zip %2 %3 -mcu=on").arg(behaviorName_).arg(htsFilePath_).arg(musicFilePath_);
        QStringList args;
        args << "a" << QString("temp\\%1.zip").arg(behaviorName_) << htsFilePath_ << musicFilePath_ << "-mcu=on";
        QProcess::execute("7z.exe",args);
//        int ret = system(convertUtf8toString(cmdStr).c_str());
//        qDebug() << "DataCenter::sendFile compress result:" << ret;
        filePath = QString("%1/temp/%2.zip").arg(QApplication::applicationDirPath()).arg(behaviorName_);
    } else {
        filePath = behaviorFilePath_;
    }
//    apolloHandler_->postFileToRobot(filePath);
    return filePath;
}

void DataCenter::sendFile(QString filePath)
{
    QFileInfo fileInfo(filePath);
    QString realPath = filePath;
    if(fileInfo.suffix().compare("xml",Qt::CaseInsensitive) == 0) {
        auto data = getDataFromBehaviorFile(filePath);
        if(data.size() > 0) {
            realPath = qApp->applicationDirPath() + "/temp/" + fileInfo.baseName() + ".hts";
            yansheeHts_->saveHtsFile(realPath,data);
        }
    }
    apolloHandler_->postFileToRobot(realPath);
}

void DataCenter::playRobotAction(const QString& actionName)
{
    apolloHandler_->putCommand(actionName,"left",1,"normal","start");
}
void DataCenter::stopRobotAction(const QString& actionName)
{
    apolloHandler_->putCommand(actionName,"left",1,"normal","stop");
}

void DataCenter::syncRobotServos()
{
    auto fullServoIds = RobotConfig->getServoApolloNames();
    apolloHandler_->getServosAngles(fullServoIds);
}

void DataCenter::onRobotServosAngles(const QMap<QString, int> &servos)
{
    QMap<int,double> oppositeServo;
    for(auto cit = servos.cbegin(); cit!=servos.cend();cit++) {
        auto servoId = RobotConfig->queryServoIdByApolloName(cit.key());
        double servoAngle = cit.value();
        currentServosData_[servoId] = servoAngle;
        if(servoMirrorEnabled(servoId)) {
            RobotConfig->calcOppositeServoValue(servoId,servoAngle);
            currentServosData_[servoId] = servoAngle;
            oppositeServo.insert(servoId,servoAngle);
        }
    }
    if(!oppositeServo.empty()) {
        putServoAnglesToRobot(oppositeServo,200);
    }
    emit changeFrameData(currentServosData_);
    if(fullFrames_.contains(currentFrameIndex_)) {
        emit keyFrameModified(true);
    }
}



void DataCenter::onChangeServoLockState(int servoId, bool isLock)
{
    QString servoName = RobotConfig->queryApolloNameByServoId(servoId);
    QList<QString> servos;
    servos.push_back(servoName);
    apolloHandler_->putServosMode(servos,isLock);
}

void DataCenter::onChangeLimbLockState(int limbId, bool isLock)
{
    auto limb = RobotConfig->queryBody(limbId);
    apolloHandler_->putServosMode(limb->getServoNames(),isLock);
}

void DataCenter::onChangeFullLockState(bool isLock)
{
    apolloHandler_->putServosMode(RobotConfig->getServoApolloNames(),isLock);
}

void DataCenter::onChangeHeadLockState(bool isLock)
{
    int headId = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_HEAD);
    onChangeLimbLockState(headId,isLock);
}

void DataCenter::onChangeHandsLockState(bool isLock)
{
    int rightHandIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_RIGHT_HAND);
    auto limb = RobotConfig->queryBody(rightHandIndex);
    auto servoNames = limb->getServoNames();
    int leftHandIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_LEFT_HAND);
    limb = RobotConfig->queryBody(leftHandIndex);
    servoNames += limb->getServoNames();
    apolloHandler_->putServosMode(servoNames,isLock);
}

void DataCenter::onChangeLegsLockState(bool isLock)
{
    int rightLegIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_RIGHT_LEG);
    auto limb = RobotConfig->queryBody(rightLegIndex);
    auto servoNames = limb->getServoNames();
    int leftLegIndex = RobotConfig->queryBodyIdByBodyType(Robotcfg::EM_BODY_LEFT_LEG);
    limb = RobotConfig->queryBody(leftLegIndex);
    servoNames += limb->getServoNames();
    apolloHandler_->putServosMode(servoNames,isLock);
}

void DataCenter::addMusic()
{
    QString musicPath = QFileDialog::getOpenFileName(nullptr,tr("Open File"),
                                                     ".",
                                                     tr("music (*.mp3)"));
    if(!musicPath.isEmpty()) {
        setFileEdited();
        musicFilePath_ = musicPath;
        emit sigAddMusic(musicFilePath_);
        emit playableStateChanged();
    }
}

void DataCenter::resetPosture()
{
    if(previewTimer_->isActive()) {
        previewTimer_->stop();
    }
    handMirrorEnabled_ = false;
    legMirrorEnabled_ = false;
    apolloHandler_->disableGetServos();
    currentServosData_ = RobotConfig->getServosInitAngle();
    playRobotAction("reset");
    emit postureReset(currentServosData_);
}

void DataCenter::changeRobotServoAngle(int servoId, double servoAngle)
{

    QMap<int,double> tempServos;
    for(int i=1;i<=17;i++){
        tempServos[i] = -1;
    }

    currentServosData_[servoId] = servoAngle;
    tempServos[servoId] = servoAngle;
    if(servoMirrorEnabled(servoId)) {
        RobotConfig->calcOppositeServoValue(servoId,servoAngle);
        currentServosData_[servoId] = servoAngle;
        tempServos[servoId] = servoAngle;
    }
    Robotsearch::getInstance()->writeAngleToRobot(tempServos,50);
//    putServoAnglesToRobot(servo,500);
}

void DataCenter::previewFrameData(int frameIndex)
{
    int previousFrame = 0;
    for(auto cit = fullFrames_.cbegin(); cit != fullFrames_.cend(); ++cit) {
        if(cit.key() < frameIndex) {
            previousFrame = cit.key();
        } else {
            break;
        }
    }
    int runtime = (frameIndex - previousFrame)*frameDivision_;

    QMap<QString,double> servosNameAngle;
    auto data = fullFrames_[frameIndex];
    for(auto cit = data.cbegin(); cit != data.cend(); ++cit) {
        int servoId = cit.key();
        double servoAngle = cit.value();
        servosNameAngle.insert(RobotConfig->queryApolloNameByServoId(servoId),servoAngle);
    }
    qDebug() << "DataCenter::changeRobotServoAnglesByFrameIndex" << servosNameAngle;
    apolloHandler_->putServosAngles(servosNameAngle,runtime);
    handMirrorEnabled_ = false;
    legMirrorEnabled_ = false;
    emit resetMirrorState();
    apolloHandler_->disableGetServos();
    currentServosData_ = data;
    emit changeFrameDataWithRunTime(data,runtime);
}

void DataCenter::sendMotionsToRobot(int frameIndex, bool firstFrame, int customInterval)
{
    if(firstFrame || fullFrames_.contains(frameIndex)) {
        QMap<int,double> data;
        int runtime = 0;
        if(getNextFrameDataAndInterval(frameIndex,data,runtime)) {
            if(customInterval != -1) {
                runtime = customInterval;
            }
//            auto servos = getServosAngleFromCompleteData(frameIndex);
            QMap<QString,double> servosNameAngle;
            for(auto cit = data.cbegin(); cit != data.cend(); ++cit) {
                servosNameAngle.insert(RobotConfig->queryApolloNameByServoId(cit.key()),cit.value());
            }
            qDebug() << "DataCenter::changeRobotServoAnglesByFrameIndex" << servosNameAngle;
            apolloHandler_->putServosAngles(servosNameAngle,runtime);
            handMirrorEnabled_ = false;
            legMirrorEnabled_ = false;
            emit resetMirrorState();
            apolloHandler_->disableGetServos();
            currentServosData_ = data;
            emit changeFrameDataWithRunTime(data,runtime);
        }
    }
}

void DataCenter::sendDataToRobot(const QMap<int,double> data, int runtime)
{
    QMap<QString,double> servosNameAngle;
    for(auto cit = data.cbegin(); cit != data.cend(); ++cit) {
        servosNameAngle.insert(RobotConfig->queryApolloNameByServoId(cit.key()),cit.value());
    }
    apolloHandler_->putServosAngles(servosNameAngle,runtime);
    apolloHandler_->disableGetServos();
    currentServosData_ = data;
    emit changeFrameDataWithRunTime(data,runtime);
}

void DataCenter::refreshActionList()
{
    apolloHandler_->getMotionsList();
}

void DataCenter::deleteActionFile(const QString& actionName)
{
    apolloHandler_->deleteMotions(actionName);
}

void DataCenter::onHandsMirrorStateChanged(bool isMirror)
{
    handMirrorEnabled_ = isMirror;
}

void DataCenter::onLegsMirrorStateChanged(bool isMirror)
{
    legMirrorEnabled_ = isMirror;
}

void DataCenter::putServoAnglesToRobot(QMap<int,double> servos, int runtime)
{
    if(servos.empty()) { return; }
//    QMap<QString,double> servoNamesAngles;
//    QSet<int> autoLockedServos;
//    for(auto cit = servos.cbegin(); cit != servos.cend(); cit++) {
//        servoNamesAngles.insert(RobotConfig->queryApolloNameByServoId(cit.key()),
//                                cit.value());
//        autoLockedServos.insert(cit.key());
//    }
    QMap<int,double> tempServos;
    for(int i=1;i<=17;i++){
        if(servos.contains(i)) {
            tempServos[i] = servos[i];
        } else {
            tempServos[i] = -1;
        }
    }
    Robotsearch::getInstance()->writeAngleToRobot(tempServos,50);
    //apolloHandler_->putServosAngles(servoNamesAngles,runtime);
}

bool DataCenter::servoMirrorEnabled(int servoId)
{
    int bodyId = RobotConfig->queryBodyIdByServoId(servoId);
    Body* body = RobotConfig->queryBody(bodyId);
    if(body != nullptr){
        int bodyType = body->getType();
        if(bodyType == Robotcfg::EM_BODY_LEFT_HAND || bodyType == Robotcfg::EM_BODY_RIGHT_HAND){
            return handMirrorEnabled_;
        } else if(bodyType == Robotcfg::EM_BODY_LEFT_LEG || bodyType == Robotcfg::EM_BODY_RIGHT_LEG) {
            return legMirrorEnabled_;
        }
    }

    return false;
}

void DataCenter::previewTemplateData(const QString& filePath)
{
    QMap<int,QMap<int,double>> tempData = getDataFromBehaviorFile(filePath);
    if(!tempData.empty()) {
        previewData_.clear();
        //移动1s，作为到起始帧的时间
        int delta = 1000/frameDivision_;
        for(auto cit = tempData.cbegin(); cit != tempData.cend(); ++cit) {
            previewData_.insert(cit.key()+delta,cit.value());
        }
        if(previewTimer_ && previewTimer_->isActive()) {
            previewTimer_->stop();
        }
        previewIndex_ = 0;
        onPreviewTimerTimeout();
        previewTimer_->start(frameDivision_);
    }
}

void DataCenter::onPreviewTimerTimeout()
{
    //和播放预览一样
    if (previewIndex_ < previewData_.lastKey()) {
        ++previewIndex_;
        if(previewIndex_ == 1 || previewData_.contains(previewIndex_)) {
            auto iter = previewData_.upperBound(previewIndex_);
            QMap<int,double> data;
            int runtime = 0;
            if(iter != previewData_.end()) {
                data = iter.value();
                runtime = (iter.key() - previewIndex_) * frameDivision();
            }
            sendDataToRobot(data,runtime);
        }
    } else {
        previewTimer_->stop();
    }
}

void DataCenter::onApplyCurrentFrameChanges()
{
    fullFrames_[currentFrameIndex_] = currentServosData_;
    emit keyFrameModified(false);
    onFramesChanged(true);
}

bool DataCenter::getLayerData()
{
    for(int i = 5; i <= 7; ++i) {
        QMap<int,QMap<int,double>>* pData;
        std::shared_ptr<BEH::ActionLayer> limbLayer;
        if(i == 5) {
            limbLayer = behaviorData_->queryHeadLayer();
            pData = &fullHeadFrames_;
        } else if(i == 6) {
            limbLayer = behaviorData_->queryHandLayer();
            pData = &fullHandFrames_;
        } else if(i == 7) {
            limbLayer = behaviorData_->queryFootLayer();
            pData = &fullFootFrames_;
        }

        if (limbLayer == nullptr) {
            return false;
        }
        auto frames = limbLayer->getFrames();
        auto groups = limbLayer->getGroups();

        pData->clear();

        for (auto itr = groups->cbegin(); itr != groups->cend(); ++itr) {
            std::string groupName = itr->first;
            std::set<int> frameList = itr->second;
            std::list<int> stdFrameList(frameList.cbegin(),frameList.cend());
            QList<int> tempFrameList = QList<int>::fromStdList(stdFrameList);
            for(int frameIndex : frameList) {
                std::shared_ptr<BEH::Frame> framePtr = (*frames)[frameIndex];
                auto servos = framePtr->getServos();
                QMap<int,double> tempServos;
                for (auto itr2 = servos->cbegin(); itr2 != servos->cend(); ++itr2) {
                    auto servoId = itr2->first;
                    std::shared_ptr<BEH::Servo> servo = itr2->second;
                    double servoAngle = servo->getTargetAngle();
                    tempServos.insert(servoId,servoAngle);
                }
                pData->insert(frameIndex,tempServos);
            }
//            QSet<int> frameIndexes = tempFrameList.toSet();
//            ActionGroup* group = new ActionGroup(QString::fromStdString(groupName),frameIndexes,this);
//            fullGroups_.insert(group->groupIndex(),group);
        }
    }
}

bool DataCenter::getDataFromBehavior()
{
    setFrameDivision(behaviorData_->getDivision());
    auto actionLayer = behaviorData_->queryActionLayer();
    if (actionLayer == nullptr) {
        return false;
    }
    auto frames = actionLayer->getFrames();
    auto groups = actionLayer->getGroups();
    for (auto itr = groups->cbegin(); itr != groups->cend(); ++itr) {
        std::string groupName = itr->first;
        std::set<int> frameList = itr->second;
        std::list<int> stdFrameList(frameList.cbegin(),frameList.cend());
        QList<int> tempFrameList = QList<int>::fromStdList(stdFrameList);
        for(int frameIndex : frameList) {
            std::shared_ptr<BEH::Frame> framePtr = (*frames)[frameIndex];
            auto servos = framePtr->getServos();
            for (auto itr2 = servos->cbegin(); itr2 != servos->cend(); ++itr2) {
                auto servoId = itr2->first;
                std::shared_ptr<BEH::Servo> servo = itr2->second;
                double servoAngle = servo->getTargetAngle();
                fullFrames_[frameIndex].insert(servoId,servoAngle);
            }
        }
        QSet<int> frameIndexes = tempFrameList.toSet();
        ActionGroup* group = new ActionGroup(QString::fromStdString(groupName),frameIndexes,this);
        fullGroups_.insert(group->groupIndex(),group);
    }
    auto musicLayer = behaviorData_->queryMusicLayer();
    auto musicItemPtr = musicLayer->queryMusicItem(0);
    if(musicItemPtr) {
        QFileInfo info(behaviorFilePath_);
        QString musicPath = info.absolutePath() + "/" + QString::fromStdString(musicItemPtr->getPath());
        if(QFile::exists(musicPath)) {
            musicFilePath_ = musicPath;

        }
    }
}

QString DataCenter::calcGroupName(QString baseName, QString separator)
{
    QList<QString> groupNames;
    for(auto group : fullGroups_) {
        groupNames.push_back(group->groupName());
    }
    QString groupName;
    for(int i = 1;;++i) {
        groupName = baseName + separator + QString::number(i);
        if(!groupNames.contains(groupName)) {
            break;
        }
    }
    return groupName;
}

int DataCenter::endFrameIndex()
{
    int endIndex = 0;
    if(fullFrames_.size() > 0) {
        endIndex = fullFrames_.lastKey();
    }
    return endIndex;
}

void DataCenter::renameGroup(int groupIndex, QString newName)
{
    auto group = fullGroups_[groupIndex];
    group->setGroupName(newName);
    setFileEdited();
}

void DataCenter::playOrPause(PlayActionType playType)
{
    //停止预览
    if(previewTimer_->isActive()) {
        previewTimer_->stop();
    }
    //已经在播放
    if (playerTimer_->isActive()) {
        playerTimer_->stop();
        if(playingMusic_) {
            emit stopMusic();
        }
        playingAction_ = false;
        playingMusic_ = false;
        emit playStateChanged(kPlayNone);
        emit playPos(0);
        if(currentFrameIndex_ < 1) {
            setCurrentFrameIndex(1);
        }
        return;
    }

    bool canPlayAction = false;
    bool canPlayMusic = false;

    switch(playType) {
    case kPlayAll:
        if(endFrameIndex_ > 0 && currentFrameIndex_ < endFrameIndex_) {
            canPlayAction = true;
        }
        if(!musicFilePath_.isEmpty() && currentFrameIndex_ < musicEndFrameIndex_
                && (endFrameIndex_ == 0 || canPlayAction)) { //
            canPlayMusic = true;
        }
        break;
    case kPlayAction:
        if(endFrameIndex_ > 0 && currentFrameIndex_ < endFrameIndex_) {
            canPlayAction = true;
        }
        break;
    case kPlayMusic:
        if(!musicFilePath_.isEmpty() && currentFrameIndex_ < musicEndFrameIndex_) {
            canPlayMusic = true;
        }
        break;
    }
    if(canPlayAction || canPlayMusic) {
        playerTimer_->setInterval(frameDivision_);
        playerTimer_->start();
        emit playStateChanged(playType);
        if(canPlayAction) {
            playingAction_ = true;
            if(fullFrames_.contains(currentFrameIndex_)) {
                --currentFrameIndex_;
                if(currentFrameIndex_ < 0) {
                    currentFrameIndex_ = 0;
                }
            }
            UBTDataCenter->sendMotionsToRobot(currentFrameIndex_,true);
        }
        if(canPlayMusic) {
            playingMusic_ = true;
            emit playMusic(currentFrameIndex_);
        }
    }
}

void DataCenter::stop(PlayActionType playType)
{
    if(previewTimer_->isActive()) {
        previewTimer_->stop();
    }
    if(playerTimer_->isActive()) {
        playerTimer_->stop();
    }
    if(playingMusic_) {
        emit stopMusic();
    }
    playingAction_ = false;
    playingMusic_ = false;
    emit playStateChanged(kPlayNone);
    setCurrentFrameIndex(1);
    emit playPos(0);
}

void DataCenter::onPlayerTimerTimeout()
{
    if(playingAction_) {
        if (currentFrameIndex_ < endFrameIndex_) {
            setCurrentFrameIndex(currentFrameIndex_+1);
            emit playPos(currentFrameIndex_*frameDivision_);
            sendMotionsToRobot(currentFrameIndex_,false);
        } else {
            playerTimer_->stop();
            if(playingMusic_) {
                emit stopMusic();
            }
            playingAction_ = false;
            playingMusic_ = false;
            emit playStateChanged(kPlayNone);
            emit playPos(0);
        }
    }
    if(playingMusic_ && !playingAction_) {
        setCurrentFrameIndex(currentFrameIndex_+1);
        emit playPos(currentFrameIndex_*frameDivision_);
    }
}

void DataCenter::onMusicEnd()
{
    if(playingMusic_ && !playingAction_ && playerTimer_->isActive()) {
        playerTimer_->stop();
        playingMusic_ = false;
        emit playStateChanged(kPlayNone);
        emit playPos(0);
    }
}

bool DataCenter::musicPlayable()
{
    return !musicFilePath_.isEmpty();
}

bool DataCenter::actionPlayable()
{
    return !fullFrames_.empty();
}

double DataCenter::getAngleForServoId(int servoId)
{
    return currentServosData_[servoId];
}

void DataCenter::emitCurrentServosData()
{
    emit changeFrameData(currentServosData_);
}

ActionGroup* DataCenter::getGroupByFrameIndex(int frameIndex)
{
    ActionGroup* group = nullptr;
    for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
        auto tempGroup = cit.value();
        if(tempGroup->contains(frameIndex)) {
            group = tempGroup;
            break;
        }
    }
    return group;
}

ActionGroup* DataCenter::getGroupByName(QString groupName)
{
    ActionGroup* group = nullptr;
    for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
        group = cit.value();
        if(group->groupName() == groupName) {
            break;
        }
    }
    return group;
}

QMap<QString,QSet<int>> DataCenter::getGroups()
{
    QMap<QString,QSet<int>> groups;
    for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
        auto tempGroup = cit.value();
        auto groupName = QString::number(tempGroup->groupIndex())+"#"+tempGroup->groupName();
        groups.insert(groupName,cit.value()->frameIndexes());
    }
    return groups;
}

void DataCenter::splitGroup()
{
    auto currentGroup = getGroupByFrameIndex(currentFrameIndex_);
    if(currentGroup == nullptr ||
            currentFrameIndex_ == currentGroup->groupIndex() ||
            currentGroup->frameIndexes().size() < 2) {
        return;
    }

    auto frameIndexes = currentGroup->frameIndexes();
    QSet<int> firstGroup;
    QSet<int> secondGroup;
    for(auto cit = frameIndexes.cbegin(); cit != frameIndexes.cend(); ++cit) {
        int tempFrameIndex = *cit;
        if(tempFrameIndex < currentFrameIndex_) {
            firstGroup.insert(tempFrameIndex);
        } else {
            secondGroup.insert(tempFrameIndex);
        }
    }

    int currentGroupIndex = currentGroup->groupIndex();
    fullGroups_.remove(currentGroupIndex);
    auto groupName = currentGroup->groupName();
    currentGroup->deleteLater();
    emit groupDeleted(currentGroupIndex);

    auto group1Name = calcGroupName(groupName,"_");
    ActionGroup* newGroup = new ActionGroup(group1Name,firstGroup,this);
    fullGroups_.insert(newGroup->groupIndex(),newGroup);
    emit createGroup(group1Name,firstGroup);

    auto group2Name = calcGroupName(groupName,"_");
    newGroup = new ActionGroup(group2Name,secondGroup,this);
    fullGroups_.insert(newGroup->groupIndex(),newGroup);
    emit createGroup(group2Name,secondGroup);

    onFramesChanged(true);
}

void DataCenter::combineSelectedGroups()
{
    auto groupList = selectedGroups_.toList();
    std::sort(groupList.begin(),groupList.end());
    int firstGroupIndex = groupList.first();
    int lastGroupIndex = groupList.last();

    //1. 确定哪些组需要怎么移动
    int delta = 0;
    QMap<int,int> combinedGroups;
    for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
        int tempGroupIndex = cit.key();
        if(tempGroupIndex > firstGroupIndex && tempGroupIndex <= lastGroupIndex) {
            if(selectedGroups_.contains(tempGroupIndex)) {
                combinedGroups.insert(tempGroupIndex,delta);
            } else {
                delta += fullGroups_[tempGroupIndex]->groupWidth();
            }
        }
    }
    //2. 生成大组
    auto firstGroup = fullGroups_[firstGroupIndex];
    auto frameIndexes = firstGroup->frameIndexes();
    QMap<int,int> movedFrames;
    for(auto cit = combinedGroups.cbegin(); cit != combinedGroups.cend(); ++cit) {
        auto tempGroupIndex = cit.key();
        auto tempGroup = fullGroups_[tempGroupIndex];
        if(delta == 0) {
            auto tempIndexes = tempGroup->frameIndexes();
            frameIndexes.unite(tempIndexes);
        } else {
            auto tempOriginList = tempGroup->frameIndexes().toList();
            std::sort(tempOriginList.begin(),tempOriginList.end());
            tempGroup->setGroupIndex(tempGroupIndex-cit.value());
            auto tempIndexes = tempGroup->frameIndexes();
            frameIndexes.unite(tempIndexes);
            auto tempList = tempIndexes.toList();
            std::sort(tempList.begin(),tempList.end());
            for(int i = 0; i < tempList.size(); ++i) {
                movedFrames.insert(tempOriginList[i],tempList[i]);
            }
        }
    }
    firstGroup->changeIndexes(frameIndexes);
    QMap<int,QSet<int>> changedGroups;
    changedGroups.insert(firstGroupIndex,frameIndexes);
    //3. 处理组
    QMap<int,ActionGroup*> tempGroups;
    QList<int> deletedGroups;
    auto lastGroup = firstGroup;
    for(auto cit = fullGroups_.cbegin(); cit != fullGroups_.cend(); ++cit) {
        auto tempGroupIndex = cit.key();
        if(tempGroupIndex <= firstGroupIndex || tempGroupIndex > lastGroupIndex) {
            tempGroups.insert(tempGroupIndex,cit.value());
        } else {
            if(combinedGroups.contains(tempGroupIndex)) {
                deletedGroups.push_back(tempGroupIndex);
                cit.value()->deleteLater();
            } else {
                int newIndex = lastGroup->lastFrameIndex()+1;
                auto tempGroup = cit.value();
                auto tempOriginList = tempGroup->frameIndexes().toList();
                std::sort(tempOriginList.begin(),tempOriginList.end());
                tempGroup->setGroupIndex(newIndex);
                auto tempIndexes = tempGroup->frameIndexes();
                auto tempList = tempIndexes.toList();
                std::sort(tempList.begin(),tempList.end());

                for(int i = 0; i < tempList.size(); ++i) {
                    movedFrames.insert(tempOriginList[i],tempList[i]);
                }
                tempGroups.insert(newIndex,tempGroup);
                changedGroups.insert(tempGroupIndex,tempIndexes);
                lastGroup = tempGroup;
            }
        }
    }
    fullGroups_.swap(tempGroups);
    QMap<int,QMap<int,double>> tempFrames;
    for(auto cit = fullFrames_.cbegin(); cit != fullFrames_.cend(); ++cit) {
        auto tempFrameIndex = cit.key();
        if(movedFrames.contains(tempFrameIndex)) {
            tempFrames.insert(movedFrames[tempFrameIndex],cit.value());
        } else {
            tempFrames.insert(tempFrameIndex,cit.value());
        }
    }
    fullFrames_.swap(tempFrames);

    for(auto groupIndex : deletedGroups) {
        emit groupDeleted(groupIndex);
    }

    for(auto cit = changedGroups.cbegin(); cit != changedGroups.cend(); ++cit) {
        emit changeGroup(cit.key(),cit.value());
    }

    onFramesChanged(true);

    selectedGroups_.clear();
    selectedGroups_.insert(firstGroupIndex);
}

void DataCenter::undoChanges()
{
    int maxPos = backupFullFrames_.size() - 1;
    if(revertIndex_ < maxPos) {
        ++revertIndex_;
    }
    revertGroupsPrivate(revertIndex_);
}

void DataCenter::redoChanges()
{
    if(revertIndex_ > 0) {
        --revertIndex_;
    }
    revertGroupsPrivate(revertIndex_);
}

void DataCenter::revertGroupsPrivate(int revertIndex)
{
    fullFrames_ = backupFullFrames_[revertIndex];
    for(auto tempGroup : fullGroups_) {
        tempGroup->deleteLater();
    }
    fullGroups_.clear();
    auto fullGroups = backupFullGroups_[revertIndex];
    for(auto cit = fullGroups.cbegin(); cit != fullGroups.cend(); ++cit) {
        auto groupName = cit.key();
        auto pos = groupName.indexOf('#');
        groupName = groupName.mid(pos+1);
        ActionGroup *group = new ActionGroup(groupName,cit.value(),this);
        fullGroups_.insert(group->groupIndex(),group);
    }
    emit playableStateChanged();
    if(!fullFrames_.empty()) {
        endFrameIndex_ = fullFrames_.lastKey();
    } else {
        endFrameIndex_ = 0;
    }
    emit totalTimeChanged(endFrameIndex_*frameDivision_);
    setFileEdited();
    emit revertGroups(backupFullGroups_[revertIndex]);
}

void DataCenter::groupPressed(int groupIndex, bool ctrl)
{
    if(!selectedFrames_.empty()) {
        selectedFrames_.clear();
        emit framesSelectionChanged(selectedFrames_);
    }
    selectionType_ = kSelectionGroup;
    bool contains = selectedGroups_.contains(groupIndex);
    lastPressedIsNew_ = !contains;
    if(!contains && ctrl) {
        selectedGroups_.insert(groupIndex);
    } else if(!contains && !ctrl) {
        selectedGroups_.clear();
        selectedGroups_.insert(groupIndex);
    }
    emit groupsSelectionChanged(selectedGroups_);
}

void DataCenter::groupReleased(int groupIndex, bool ctrl)
{
    if(ctrl && !lastPressedIsNew_) {
        selectedGroups_.remove(groupIndex);
    } else if(!ctrl){
        selectedGroups_.clear();
        selectedGroups_.insert(groupIndex);
    }
    if(selectedGroups_.empty()) {
        selectionType_ = kSelectionNone;
    }
    emit groupsSelectionChanged(selectedGroups_);
}

bool DataCenter::framePressed(int frameIndex, bool ctrl)
{
    if(!selectedGroups_.empty()) {
        selectedGroups_.clear();
        emit groupsSelectionChanged(selectedGroups_);
    }
    selectionType_ = kSelectionFrame;
    bool contains = selectedFrames_.contains(frameIndex);
    lastPressedIsNew_ = !contains;
    bool sameGroup = true;
    if(!selectedFrames_.empty()) {
        auto currentGroup = getGroupByFrameIndex(*selectedFrames_.begin());
        auto frameGroup = getGroupByFrameIndex(frameIndex);
        sameGroup = (frameGroup == currentGroup);
    }
    if(!contains && ctrl && sameGroup) { //不允许跨组选择
        selectedFrames_.insert(frameIndex);
        return false;
    } else if(!contains && !ctrl) {
        selectedFrames_.clear();
        selectedFrames_.insert(frameIndex);
    }
    emit framesSelectionChanged(selectedFrames_);
    return true;
}

void DataCenter::frameReleased(int frameIndex, bool ctrl)
{
    if(ctrl && !lastPressedIsNew_) {
        selectedFrames_.remove(frameIndex);
    } else if(!ctrl){
        selectedFrames_.clear();
        selectedFrames_.insert(frameIndex);
    }
    if(selectedFrames_.empty()) {
        selectionType_ = kSelectionNone;
    }
    emit framesSelectionChanged(selectedFrames_);
}

void DataCenter::clearSelection()
{
    if(!selectedGroups_.empty()) {
        selectedGroups_.clear();
        emit groupsSelectionChanged(selectedGroups_);
    }
    if(!selectedFrames_.empty()) {
        selectedFrames_.clear();
        emit framesSelectionChanged(selectedFrames_);
    }
    emit clearMusicSelection();
    selectionType_ = kSelectionNone;
}

void DataCenter::selectGroups(QSet<int> groups)
{
    if(!selectedFrames_.empty()) {
        selectedFrames_.clear();
        emit framesSelectionChanged(selectedFrames_);
    }
    if(groups.empty()) {
        selectionType_ = kSelectionNone;
    } else {
        selectionType_ = kSelectionGroup;
    }
    selectedGroups_ = groups;
    emit groupsSelectionChanged(selectedGroups_);
}

void DataCenter::selectFrames(QSet<int> frames)
{
    if(!selectedGroups_.empty()) {
        selectedGroups_.clear();
        emit groupsSelectionChanged(selectedGroups_);
    }
    if(frames.empty()) {
        selectionType_ = kSelectionNone;
    } else {
        selectionType_ = kSelectionFrame;
    }
    selectedFrames_ = frames;
    emit framesSelectionChanged(selectedFrames_);
}

void DataCenter::selectMusic()
{
    if(!selectedGroups_.empty()) {
        selectedGroups_.clear();
        emit groupsSelectionChanged(selectedGroups_);
    }
    if(!selectedFrames_.empty()) {
        selectedFrames_.clear();
        emit framesSelectionChanged(selectedFrames_);
    }
    selectionType_ = kSelectionMusic;
    emit timeLineChanged();
}

void DataCenter::selectAllGroups()
{
    if(!selectedFrames_.empty()) {
        selectedFrames_.clear();
        emit framesSelectionChanged(selectedFrames_);
    }
    selectionType_ = kSelectionGroup;
    selectedGroups_ = fullGroups_.keys().toSet();
    emit groupsSelectionChanged(selectedGroups_);
}


void DataCenter::onConnectionChanged(int state)
{
    emit robotConnectionStateChanged(state,mSn);
}

void DataCenter::checkFramesInvterval()
{
    if(fullFrames_.size() < 2) {
        return;
    }
    int lastFrameIndex = fullFrames_.firstKey();
    bool over4000ms = false;
    for(auto cit = fullFrames_.cbegin(); cit != fullFrames_.cend(); ++cit) {
        int frameIndex = cit.key();
        int timeInterval = (frameIndex - lastFrameIndex)*frameDivision_;
        if(timeInterval > 4000) {
            over4000ms = true;
            break;
        }
        lastFrameIndex = frameIndex;
    }

    if(over4000ms) {
        ToastDialog *toast = new ToastDialog();
        toast->setDisplayText(tr("robot will not execute frames that interval over 4000ms"));
        toast->show();
    }
}

void DataCenter::sendAdjustServo(int selectServoId)
{
    emit sigAdjustServo(selectServoId);
}


