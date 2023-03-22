#ifndef SERVOFORM_H
#define SERVOFORM_H

#include <QWidget>
#include"dangerousdialog.h"
#include"classstudiosettings.h"
#include"datacenter.h"
#include<QTimer>

namespace Ui {
class ServoForm;
}

class ServoForm : public QWidget
{
    Q_OBJECT

public:
    explicit ServoForm(QWidget *parent = nullptr);
    ~ServoForm();
    void setId(int id);
    void setBodyId(int bodyId);
    void setName(const QString& name);
    void setValue(double value);
    void setMaxValue(double value);
    void setMinValue(double value);
    void configStyle();
    void setLockState(bool isLock);
    int getServoId();
    void setSelected(bool isSelected);
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void onRobotConnectStateChange(int state, QString sn);
    void onPlayStateChange(DataCenter::PlayActionType playType);
signals:
    void servoAngleChanged(int,double);
    void servoAngleChangeFinished(int,double);
    void servoLockStateChanged(int,bool);
    void sigServoSelect(int servoId);


private:
    Ui::ServoForm *ui;
    bool isProgramSet_;
    bool sliderPressed_;
    int servoAngleBeforePressed_;
    int servoId_;
    int servoAngle_;
    bool servoLocked_;
    int mBodyId;
    DangerousDialog *dDialog;
    QString locked_url = ":/res/images/jointAdjustment/ic_lock_device.png";
    QString unlocked_url = ":/res/images/jointAdjustment/ic_unlock_device.png";
    QString disenable_url = ":/res/images/jointAdjustment/ic_lock_device_disable.png";
    int mConnectState = 0;
    QTimer *editTiemr = nullptr;


};

#endif // SERVOFORM_H
