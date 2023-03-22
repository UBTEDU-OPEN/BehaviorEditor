#ifndef VIRTUALROBOTBOTTOMWIDGET_H
#define VIRTUALROBOTBOTTOMWIDGET_H

#include <QWidget>
#include<QPushButton>
#include<settimedialog.h>
#include<QString>

#include "datacenter.h"

namespace Ui {
class VirtualRobotBottomWidget;
}

class VirtualRobotBottomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualRobotBottomWidget(QWidget *parent = nullptr);
    ~VirtualRobotBottomWidget();
    void resizeEvent(QResizeEvent *event);
    void setModifyBtnEnabled(bool enabled);
    
signals:
    void modifyBtnClicked();
    void resetBtnClicked();
    void addBtnClicked();
    void frameIntervalChanged(int);

private slots:
    void onSetTimeDialogClose();
    void onSetTimeBtnClick();
    void onAddBtnClicked();
    void on_resetBtn_clicked();
    void on_modifyBtn_clicked();
    void on_playBtn_clicked();
    void on_stopBtn_clicked();
    void onPlayableStateChanged();
    void onPlayStateChange(DataCenter::PlayActionType playType);

private:
    Ui::VirtualRobotBottomWidget *ui;
    QPushButton *setTimeBtn;
    SetTimeDialog *setTimeDialog;
    int frameInterval_;
};

#endif // VIRTUALROBOTBOTTOMWIDGET_H
