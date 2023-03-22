#ifndef ROBOTACTIONLIST_H
#define ROBOTACTIONLIST_H

#include <QWidget>

#include "robotactionitem.h"
#include<robotactionlistitem.h>

namespace Ui {
class RobotActionList;
}

class RobotActionList : public QWidget
{
    Q_OBJECT

public:
    explicit RobotActionList(QWidget *parent = nullptr);
    ~RobotActionList();
    void clearList();
    void createNewList(const QList<QString>& actionNames);
public slots:
    void robotConnectionStateChanged(int state, QString sn);
private:
    Ui::RobotActionList *ui;
    QList<RobotActionListItem*> *actionList_ = NULL;
    QString selectedItemName_;
    QString triggeredItemName_;
    void setBtnDisable(bool isDisable);
};

#endif // ROBOTACTIONLIST_H
