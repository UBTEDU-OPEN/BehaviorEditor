#ifndef JOINTADJUESTMENTWIDGET_H
#define JOINTADJUESTMENTWIDGET_H

#include <QWidget>
#include "servoeditor_global.h"
#include<QVector>
#include"servoform.h"
#include<QMap>

namespace Ui {
class JointAdjuestmentWidget;
}

class SERVOEDITOR_EXPORT JointAdjuestmentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JointAdjuestmentWidget(QWidget *parent = nullptr);
    ~JointAdjuestmentWidget();

signals:
    void mirrorBtnOpen(bool isOpened);
    void servoAngleChanged(int,double);
    void servoAngleChangeFinished(int,double);
    void handsMirrorStateChanged(bool);
    void legsMirrorStateChanged(bool);
    void sigServoSelected(int servoId);
public slots:
    void onJointSelected(int joint,bool isSelected);
    void onJointLocked(int joint,bool isLocked);
    void onServoValueChanged(int id, double value);
    void onServoLockStateChanged(int id, bool isLocked);
    void onSigCurrentSelectServoId(int nId, int nBody);
    void onFrameIdChanged(const QMap<int, double> &servos);
    void resetLockState();
    void resetMirrorState();
    void onPostureReset(const QMap<int,double>& servos);
    void onPlayStateChange(DataCenter::PlayActionType playType);
    void onServoSelect(int servoId);

private:
    Ui::JointAdjuestmentWidget *ui;
    int mCurrJointId = 0;
    int mCurrSelectServoId = 0;
    QVector<ServoForm*> mServoWidgets;
    QMap<int, double> mServosValue;
    QMap<int,bool> mServosLockFlag;
    bool mHandMirrorEnabled;
    bool mLegMirrorEnabled;
    void reverseCurrentLimbMirrorEnabled();
    void updateServosInfo(int joint);
    void createServoUI();
    void setMirrorBtnVisible(bool visible);
    void servoSelect(int servoId);
    void lockedForJointId(int joint,bool isLocked);
    int getMirrorServoId(int servoId);
    void setServorForMirrorBtn(bool isHand);

};

#endif // JOINTADJUESTMENTWIDGET_H
