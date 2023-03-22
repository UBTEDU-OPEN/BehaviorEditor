#ifndef ROBOTVIEWCHANGEWIDGET_H
#define ROBOTVIEWCHANGEWIDGET_H

#include <QWidget>
#include<QMouseEvent>
#include"viewchangebutton.h"

namespace Ui {
class RobotViewChangeWidget;
}

class RobotViewChangeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RobotViewChangeWidget(QWidget *parent = nullptr);
    ~RobotViewChangeWidget();
    void setDirection(Direction direction);
signals:
    void selectDirection(int dir);
private:
    Ui::RobotViewChangeWidget *ui;
};

#endif // ROBOTVIEWCHANGEWIDGET_H
