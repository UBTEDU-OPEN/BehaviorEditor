#ifndef JOINTSELECTORWIDGET_H
#define JOINTSELECTORWIDGET_H

#include <QWidget>

namespace Ui {
class JointSelectorWidget;
}
enum Joint{
   HEADER=1,
   RIGHTARM=2,
   LEFTARM=3,
   RIGHTLEG=4,
   LEFTLEG=5,
};
class JointSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JointSelectorWidget(QWidget *parent = nullptr);
    ~JointSelectorWidget();
    void setJointName(int joint,QString jointName);
    void resetLockState();
    void setAllServorsUnlocked(bool isAllUnlocked,int jonitId);
    bool getIsLockedForJoint(int jointId);

public slots:
    void onHeadClicked();
    void onLeftArmClicked();
    void onRightArmClicked();
    void onLeftLegClicked();
    void onRightLegClicked();
signals:
    void jointSelected(Joint joint,bool isSelected);
    void jointLocked(Joint joint,bool isLocked);
private:
    Ui::JointSelectorWidget *ui;
    Joint currSelectedJoint;
    int mRobotConnectState;
    void initConnect();
};

#endif // JOINTSELECTORWIDGET_H
