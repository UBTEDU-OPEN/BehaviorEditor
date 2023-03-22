#ifndef SERVOEDITOR_H
#define SERVOEDITOR_H

#include <QWidget>
#include <QWheelEvent>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QStringList>
#include <QMenu>
#include <QMap>
#include <QCheckBox>
#include <QVector>

#include "servoeditor_global.h"
#include "combobutton.h"
#include "commondialog.h"
#include "servoform.h"

namespace Ui {
	class ServoEditor;
}

class SERVOEDITOR_EXPORT ServoEditor: public QWidget
{
    Q_OBJECT
public:
	explicit ServoEditor(QWidget *parent = nullptr);
	~ServoEditor();

    void setFullLockState(bool isLocked);
    void setHeadLockState(bool isLocked);
    void setHandsLockState(bool isLocked);
    void setLegsLockState(bool isLocked);
    void setLimbLockState(int limbId, bool isLocked);
        
    void onLockBtnClicked();

signals:
    void sigAddFrame(int,int);
    void servoAngleChanged(int,double);
    void servoAngleChangeFinished(int,double);
    void editFrameBtnClicked();
    void autoSaveEnabled(bool);
    void inserFrame();
    void handsMirrorStateChanged(bool);
    void legsMirrorStateChanged(bool);
    void resetBtnClicked();
        
public slots:
	void onRightArmClicked();
	void onLeftArmClicked();
	void onRightLegClicked();
	void onLeftLegClicked();
	void onHeadClicked();
        
         
	void onEditFrameBtnClicked();
	void onEditAllActionTrigger();
	void onEditRightArmActionTrigger();
	void onEditLeftArmActionTrigger();
	void onEditRightLegActionTrigger();
	void onEditLeftLegActionTrigger();
	void onEditHeadActionTrigger();
        
	/**
	 * @brief onFrameIdChanged 帧ID改变
	 * @param frameIndex 帧ID
	 * @param servoInfo <servoId,<servoValue,runtime>>
	 */
    void onFrameIdChanged(QMap<int, QPair<double, int>> servoInfo, bool isKeyFrame);
	/**
	 * @brief on3DValueChanged 3D舵机改变
	 * @param id 舵机ID
	 * @param value 舵机数值
	 */
    void onSingleServoAngleChanged(int id, double value);
        
    void onServoSelectinChanged(int nId, int nBody);

    void onPlayingStatusChanged(bool isPlaying);

    void onServosAutoLocked(QSet<int> servoIds);

    void onResetMirrorState();
        
protected:
	void wheelEvent(QWheelEvent *event) override;
        
private:
	Ui::ServoEditor *ui;
    int currentLimbIndex_;
    int maxServoCount_;
    bool isPlaying_;
    bool autoSave_;
    QMap<int,bool> servosLockFlag_;
    QMenu *editMenu_;
    QAction *editFullBodyAction_;
    QAction *editRightArmAction_;
    QAction *editLeftArmAction_;
    QAction *editRightLegAction_;
    QAction *editLeftLegAction_;
    QAction *editHeadAction_;
    QVector<ServoForm*> servoWidgets_;
    QMap<int, double> servosValue_;
    QMap<int,int> limbsServoCount_;
    bool handMirrorEnabled_;
    bool legMirrorEnabled_;

private:
    void createServoUI();
    void initConnection();
	void updateServosInfo();
	void onServoValueChanged(int id, double value);
    void onServoLockStateChanged(int id, bool isLocked);
    void onEditActionTriggered(int bodyType);
    bool isFullLocked();
    bool isLimbLocked(int limbId);
    bool isHeadLocked();
    bool isHandsLocked();
    bool isLegsLocked();
    bool isCurrentLimbMirrorEnabled();
    void reverseCurrentLimbMirrorEnabled();
};

#endif // SERVOEDITOR_H
