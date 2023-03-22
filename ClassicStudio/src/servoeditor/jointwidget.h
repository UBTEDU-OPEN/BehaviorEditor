#ifndef JOINTWIDGET_H
#define JOINTWIDGET_H

#include <QWidget>
#include<QPoint>
#include<QMouseEvent>
#include"dangerousdialog.h"
#include"datacenter.h"

namespace Ui {
class JointWidget;
}

class JointWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JointWidget(QWidget *parent = nullptr);
    ~JointWidget();
    void setJointName(QString jointName);
    void setJointId(int jointId);
    void setSelected(bool isSelected);
    void setLocked(bool locked);
    bool getLocked();
    void allServorsUnlocked(bool allUnlocked);

private slots:
    void onSigSure();
    void onRobotConnectStateChange(int state, QString sn);
    void onPlayStateChange(DataCenter::PlayActionType playType);
signals:
    void clicked(bool isSelected);
    void lockBtnClicked(bool isLocked);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);


private:
    Ui::JointWidget *ui;
    bool isSelected;
    bool isLocked;
    QPoint mousePos;
    int mRobotConnectState;
    int mJointId;
    DangerousDialog *dDialog;
    QString style_sheet_enabled = "QWidget#lockBtn{border: none;background: url(:/res/images/jointAdjustment/ic_lock_device_disable.png);}";
    QString style_sheet_unlocked = "QWidget#lockBtn{border: none;background: url(:/res/images/jointAdjustment/ic_unlock_device.png);}";
    QString style_sheet_locked = "QWidget#lockBtn{border: none;background: url(:/res/images/jointAdjustment/ic_lock_device.png);}";
};

#endif // JOINTWIDGET_H
