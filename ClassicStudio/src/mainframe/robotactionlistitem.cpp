#include "robotactionlistitem.h"
#include "ui_robotactionlistitem.h"
#include<QDebug>

RobotActionListItem::RobotActionListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotActionListItem)
{
    ui->setupUi(this);
    ui->widget->installEventFilter(this);
}

RobotActionListItem::~RobotActionListItem()
{
    delete mItem;
    delete ui;
}

void RobotActionListItem::setActionName(QString actionName)
{
    this->actionName_ = actionName;
    ui->actionNameLabel->setText(actionName);
}

QString RobotActionListItem::getActionName()
{
    return actionName_;
}

void RobotActionListItem::setSelcetStyle(bool isSelect)
{
    if(isSelect){
        ui->widget->setStyleSheet("QWidget#widget{border: 1px solid #00A5FF;}");
    }else {
        ui->widget->setStyleSheet("QWidget#widget{border: node;}");
    }
}

QListWidgetItem *RobotActionListItem::getItem()
{
    return mItem;
}

void RobotActionListItem::setItem(QListWidgetItem *item)
{
    this->mItem = item;
}



bool RobotActionListItem::eventFilter(QObject *watched, QEvent *event)
{
    // 你要过滤的对象
    if (watched == ui->widget) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            // 你自己期望实现的功能，在这里我的实现是新建一个标签页以及新的文本编辑栏
            qDebug() << "double click===";
            emit itemTriggered(actionName_);
            return true; // 注意这里一定要返回true，表示你要过滤该事件原本的实现
        }else if(event->type() == QEvent::MouseButtonPress){
            emit itemSelected(actionName_);

            return true;
        }
    }

    return false; // 返回false表示不过滤，还按默认的来
}
