#ifndef ROBOTSEARCHDIALOG_H
#define ROBOTSEARCHDIALOG_H

#include <QDialog>
#include "robotsearch_global.h"
#include"inputsnwidget.h"
#include<QString>
#include"robotsearch.h"
#include<QCloseEvent>
#include"apollocommunication.h"
#include"robotlistwidget.h"
#include"connectionsuccesswidget.h"
#include"classstudiosettings.h"

namespace Ui {
class RobotSearchDialog;
}

class ROBOTSEARCH_EXPORT RobotSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RobotSearchDialog(int connectionState = 0, QWidget *parent = nullptr);
    ~RobotSearchDialog();
    void closeEvent(QCloseEvent *event);
    void setApolloCommunication(ApolloCommunication * communication);
    void setSn(QString sn);
signals:
    void connectStateChanged(int state,const QString &sn);
private slots:
    void onConnectBtnClicked(const QString &sn);
    void onSearchRobotResult(QList<RobotInfo> *result);
    void onSearchTimeOut();
    void onRobotConnectionStateChanged(int state);

    void on_closeBtn_clicked();

private:
    Ui::RobotSearchDialog *ui;
    RobotListWidget *listWidget;
    ApolloCommunication *mCommunication;
    QString mSn;
    RobotInfo mConnectRobotInfo;
    int mConnectionState;
};

#endif // ROBOTSEARCHDIALOG_H
