#ifndef DATACENTER_H
#define DATACENTER_H

#include "datacenter_global.h"

#include <QObject>
#include <QTimer>

#include "actiongroup.h"

class YansheeHts;
class YansheeLayer;
class ApolloCommunication;
class RobotInfo;
class Robotsearch;
namespace BEH {
    class Behavior;
}

#define UBTDataCenter DataCenter::getInstance()

class DATACENTER_EXPORT DataCenter : public QObject
{
    Q_OBJECT
public:
    enum CopyType {
        kCopyNone = 0,
        kCopyFrame,
        kCopyGroup
    };

    enum SelectionType {
        kSelectionNone = 0,
        kSelectionFrame,
        kSelectionGroup,
        kSelectionMusic
    };

    enum PlayActionType {
        kPlayAll = 0,
        kPlayAction,
        kPlayMusic,
        kPlayNone
    };

    static const int kInvalidFrameIndex = -1;
    static DataCenter* getInstance();
    void init();
    bool isFileEdited() { return fileEdited_; }

    //状态
    int frameDivision();
    int frameRate();
    void setFrameDivision(int frameDivision);

    //帧数据和音乐
    bool newBehaviorData();
    bool loadBehaviorData(const QString& filePath);
    bool saveBehaviorData(bool saveAs = false);
    const QMap<int,QMap<int,double>>& fullFrames() { return fullFrames_; }
    void deleteMusic();
    QMap<int, QMap<int, double> > getDataFromBehaviorFile(const QString &filePath);
    void changeRobotServoAngle(int servoId, double servoAngle);
    void addMusic();
    QString getMusicFilePath() { return musicFilePath_; }
    void saveDataToTemplate(int groupIndex);

    void addFrame();
    QString behaviorFilePath() { return behaviorFilePath_; }
    void deleteSelectedFrames();
    void deleteSelectedGroups();
    void pasteCopiedFrames();
    void pasteCopiedFramesInGroup(int groupIndex, int destIndex);
    void copySelectedGroupsData();
    void copyFramesData();
    void pasetCopiedGroups();
    void setCurrentFrameIndex(int index);
    void setDeviceVolume(int volume);
    void insertGroupData(QString groupName, QMap<int,QMap<int,double>> data, int targetIndex);
    void previewTemplateData(const QString& filePath);
    void moveFrames(QMap<int,int> movedFrames);
    void moveGroup(int groupIndex, int targetIndex);
    void changeCurrentFrameServo(int servoId, double servoAngle);
    void servoAngleChangeFinished(int servoId, double servoAngle);
    void onChangeServoLockState(int servoId, bool isLock);
    void onChangeLimbLockState(int limbId, bool isLock);
    void onChangeFullLockState(bool isLock);
    void onChangeHeadLockState(bool isLock);
    void onChangeHandsLockState(bool isLock);
    void onChangeLegsLockState(bool isLock);
    void onHandsMirrorStateChanged(bool isMirror);
    void onLegsMirrorStateChanged(bool isMirror);
    void onPreviewTimerTimeout();
    void onApplyCurrentFrameChanges();
    CopyType getCopyType() { return copyType_; }
    int endFrameIndex();
    void renameGroup(int groupIndex, QString newName);
    void insertTemplateFileData(int frameIndex, const QString &filePath);
    void setAddFrameInterval(int interval) { addFrameInterval_ = interval; }
    QMap<QString,QSet<int>> getGroups();
    //组及撤销
    void splitGroup();
    void combineSelectedGroups();



    //物理机器人相关
    void connectRobot();
    QString compressCurrentBehavior();
    void sendFile(QString filePath);
    void syncRobotServos();
    void resetPosture();
    void sendMotionsToRobot(int frameIndex, bool firstFrame, int customInterval = -1);
    void previewFrameData(int frameIndex);
    void playRobotAction(const QString& actionName);
    void stopRobotAction(const QString& actionName);
    void refreshActionList();
    void deleteActionFile(const QString& actionName);
    int getBatteryChargingState() { return batteryCharging_; }
    int getBatteryPercent() { return batteryPercent_; }
    int getVolume() { return volume_; }
	int getCurrentConnectState();
    void onRobotServosAngles(const QMap<QString,int>&);
    void changeCurrentFrameServoNotToRobot(int servoId,double angel);
    void onConnectionChanged(int state);
    //播放相关
    bool isPlaying() { return isPlaying_; }
    bool getNextFrameDataAndInterval(int frameIndex, QMap<int,double>& data, int& interval);
    void playOrPause(PlayActionType playType);
    void stop(PlayActionType playType);
    void onMusicEnd();
    void setMusicEndFrameIndex(int index) { musicEndFrameIndex_ = index; }
    bool musicPlayable();
    bool actionPlayable();
    double getAngleForServoId(int servoId);
    void emitCurrentServosData();
    //selection
    QSet<int> getSelectedFrames() { return selectedFrames_; }
    QSet<int> getSelectedGroups() { return selectedGroups_; }

    void groupPressed(int groupIndex, bool ctrl);
    void groupReleased(int groupIndex, bool ctrl);
    bool framePressed(int frameIndex, bool ctrl);
    void frameReleased(int frameIndex, bool ctrl);
    void clearSelection();
    void selectGroups(QSet<int> groups);
    void selectFrames(QSet<int> frames);
    void selectMusic();
    void selectAllGroups();
    //
    void undoChanges();
    void redoChanges();
    SelectionType getSelectionType() { return selectionType_; }

    bool servoMirrorEnabled(int servoId);

    void checkFramesInvterval();

    void sendAdjustServo(int sendAdjustServo);


signals:
    void newBehavior();
    void changeFrameData(const QMap<int, double> &servos);
    void changeFrameDataWithRunTime(const QMap<int,double>& servos, int runtime);
    void currentFrameServoChanged(int servoId, double servoAngle);
    void sigAddMusic(const QString&);
    void robotConnectionStateChanged(int state, QString sn);
    void robotActionList(const QList<QString>& actionList);
    void currentFrameIndexChanged(int frameIndex);
    void resetLockState();
    void resetMirrorState();
    void chargingStateNotify(int);
    void batteryPercentNotify(int);
    void volumeNotify(int);
    void playMusic(int);
    void stopMusic();
    void playStateChanged(PlayActionType);
    void addFileToTemplate(const QString&);
    void totalTimeChanged(int);
    void fileOpend(const QString&);
    void fileSaved(const QString&);
    void fileEdited();
    void postureReset(const QMap<int,double>& servos);
    void sendFileResult(bool result);
    void frameDivisionChanged(int);
    void keyFrameModified(bool);
//    void groupChanged(QMap<QString,QSet<int>> groups, int selectFrameIndex);
    void playableStateChanged();
    void playPos(int); //milliseconds
    void moveGroupsBehind(QSet<int>,int);
    //初始groupIndex,新的frameIndexes
    void changeGroup(int,QSet<int>);
    void groupDeleted(int);
    void createGroup(QString,QSet<int>);

    //
    void framesSelectionChanged(QSet<int>);
    void groupsSelectionChanged(QSet<int>);
    void clearMusicSelection();
    void revertGroups(QMap<QString,QSet<int>> fullGroups);
    //
    void timeLineChanged();

    void sigAdjustServo(int selectServoId);

private:
    explicit DataCenter(QObject* parent = nullptr);
    std::string convertUtf8toString (const QString& filePath);
    QString calcFormatTimeString(int startTime,bool withFrame = true);
    bool saveBehaviorData(const QString& filePath);
    QString getFileName();
    void putServoAnglesToRobot(QMap<int,double> servos, int runtime);

    void setFileEdited();
    bool getDataFromBehavior();
    bool getLayerData();
    QString calcGroupName(QString baseName, QString separator = "");
    void sendDataToRobot(const QMap<int,double> data, int runtime);
    void onFramesChanged(bool fileEdited);
    void onPlayerTimerTimeout();
    ActionGroup* getGroupByFrameIndex(int frameIndex);
    ActionGroup* getGroupByName(QString groupName);
    void insertFrame(ActionGroup* group, int destIndex, QMap<int,double> data);
    void revertGroupsPrivate(int revertIndex);
    void saveDataToTemplatePrivate(int groupIndex,QString templateName);

private:
    int currentFrameIndex_;
    int endFrameIndex_;
    QMap<int,QMap<int,double>> fullFrames_;
    QMap<int,QMap<int,double>> fullHeadFrames_;
    QMap<int,QMap<int,double>> fullHandFrames_;
    QMap<int,QMap<int,double>> fullFootFrames_;
    QMap<int,ActionGroup*> fullGroups_;
    bool isPlaying_;
    int servoCount_;
    YansheeHts* yansheeHts_;
    QString musicFilePath_;
    QString behaviorFilePath_;
    QString htsFilePath_;
    QString behaviorName_;
    ApolloCommunication* apolloHandler_;
    bool handMirrorEnabled_;
    bool legMirrorEnabled_;
    QString mSn;
    int currentConnectState;
    int batteryPercent_;
    int batteryCharging_;
    int volume_;
    int addFrameInterval_;
    QMap<int,QMap<int,double>> copiedFramesData_;
    QMap<int,double> currentServosData_;
    bool fileEdited_;
    QTimer* previewTimer_;
    int previewIndex_;
    QMap<int,QMap<int,double>> previewData_;
    bool keyFrameModified_;
    BEH::Behavior* behaviorData_;
    int frameDivision_;
    CopyType copyType_;
    QTimer *playerTimer_;
    PlayActionType playType_;
    int musicEndFrameIndex_;
    bool playingAction_;
    bool playingMusic_;

    QMap<int,QPair<QString,QMap<int,QMap<int,double>>>> copiedGroupsData_;

    SelectionType selectionType_;
    QSet<int> selectedFrames_;
    QSet<int> selectedGroups_;
    bool lastPressedIsNew_; //记录上一个是否是新选择，方便release时判断


    QList<QMap<int,QMap<int,double>>> backupFullFrames_;
    QList<QMap<QString,QSet<int>>> backupFullGroups_;
    int revertIndex_;

    YansheeLayer* yansheeLayer_;
};

#endif // DATACENTER_H
