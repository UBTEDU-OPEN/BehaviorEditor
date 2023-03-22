#ifndef ROBOTLISTWIDGET_H
#define ROBOTLISTWIDGET_H

#include <QWidget>
#include<QList>
#include"robotsearch.h"
#include<QLabel>
#include<QPushButton>
#include<QHBoxLayout>

namespace Ui {
class RobotListWidget;
}

class RobotListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RobotListWidget(QWidget *parent = nullptr);
    ~RobotListWidget();
    void setRobotinfoList(QList<RobotInfo> *robotList);
    QList<RobotInfo> diffRobotInfos(QList<RobotInfo> *robotinfos);
signals:
    void listConnectionBtnClick(const RobotInfo &info);
private:
    Ui::RobotListWidget *ui;
    QList<RobotInfo> *mInfos;
    int mIndex;
};

#endif // ROBOTLISTWIDGET_H
