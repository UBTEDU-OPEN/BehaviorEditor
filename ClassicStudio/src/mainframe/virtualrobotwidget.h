#ifndef VIRTUALROBOTWIDGET_H
#define VIRTUALROBOTWIDGET_H

#include <QWidget>
#include"virtualrobotbottomwidget.h"
#include"robotviewchangewidget.h"

namespace Ui {
class VirtualRobotWidget;
}

class VirtualRobotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualRobotWidget(QWidget *parent = nullptr);
    ~VirtualRobotWidget();

signals:
    void modify();
    void sigCurrentSelectServoId(int nId, int nBody);
    void sigServoAngleChange (int nId, double dAngle);
    void addFrame();
    void resetBtnClicked();
    void frameIntervalChanged(int);

public slots:
    void onSelectDirection(int dir);
    void onJointVauleChange(int,double);
    void onSetFramAglne(const QMap<int, double> &servos);
    void onSetFramAngleWithRunTime(const QMap<int, double> &servos, int runTime);
    void onPostureReset(const QMap<int,double>& servos);
    void onKeyFrameModified(bool);
    void onServoSelected(int servoId);

private:
    Ui::VirtualRobotWidget *ui;
    RobotViewChangeWidget *viewChangeWidget;
};

#endif // VIRTUALROBOTWIDGET_H
