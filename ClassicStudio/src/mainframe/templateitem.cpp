#include "templateitem.h"
#include "ui_templateitem.h"
#include<QDebug>
#include"datacenter.h"
#include<QMouseEvent>

TemplateItem::TemplateItem(QString actionName,QWidget *parent) :
    QWidget(parent)
    ,mActionName(actionName)
    ,ui(new Ui::TemplateItem)
{
    setAttribute(Qt::WA_StyledBackground);
    ui->setupUi(this);

    ui->actionNameLabel->setText(actionName);
    ui->iconLabel->setFixedSize(QSize(24,24));
    ui->widget->installEventFilter(this);
}

TemplateItem::~TemplateItem()
{
    delete ui;
}

QString TemplateItem::getActionName()
{
    return mActionName;
}

void TemplateItem::setActionName(QString actionName)
{
    this->mActionName = actionName;
    ui->actionNameLabel->setText(actionName);
}

void TemplateItem::setSelected(bool selected)
{

    if(selected){
        ui->widget->setStyleSheet("QWidget#widget{border: 1px solid #00A5FF;}");
    }else {
        ui->widget->setStyleSheet("QWidget#widget{border: node;}");
    }

}

void TemplateItem::setFiles(QList<QString> files)
{
    this->m_files = files;
}

bool TemplateItem::eventFilter(QObject *watched, QEvent *event)
{
    // 你要过滤的对象
    if (watched == ui->widget) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            // 你自己期望实现的功能，在这里我的实现是新建一个标签页以及新的文本编辑栏
            qDebug() << "double click===";
            QString filePath = m_files.at(0);
            DataCenter::getInstance()->previewTemplateData(filePath);
            return true; // 注意这里一定要返回true，表示你要过滤该事件原本的实现
        }else if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if(mouseEvent->button() == Qt::LeftButton){
                QPoint point = mouseEvent->globalPos();
                QPoint pos = mouseEvent->pos();
                mPressPoint = point;
                qDebug() << "point===" << point << ";pos==" << pos;
                startLongPress();
                emit selectedItem(mIndex);
                return true;
            }
        }else if(event->type() == QEvent::MouseButtonRelease){

            if(longPressStarted){
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                QPoint point = mouseEvent->globalPos();
                QString filePath = m_files.at(0);
                emit longPressReleaseEvent(point,filePath);

            }
            stopLongPress();
            return true;

        }else if(event->type() == QEvent::MouseMove){
            if(longPressStarted){
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

                QPoint point = mouseEvent->globalPos();

                emit longPressMoveEvent(point);

                return true;
            }

        }
    }

    return false; // 返回false表示不过滤，还按默认的来

}

void TemplateItem::setIndex(int index)
{
    this->mIndex = index;
}

int TemplateItem::getIndex()
{
    return mIndex;
}

void TemplateItem::startLongPress()
{
    mTimer = new QTimer(this);
    mTimer->setSingleShot(true);
    connect(mTimer,&QTimer::timeout,this,&TemplateItem::onLongPress);
    mTimer->start(800);
}

void TemplateItem::stopLongPress()
{
    if(mTimer){
        longPressStarted = false;
        mTimer->stop();
        delete mTimer;
        mTimer = nullptr;
    }
}

void TemplateItem::onLongPress()
{
    qDebug() << "onLongPress===";
    QString filePath = m_files.at(0);
    qDebug() << "filePath====" << filePath;
    longPressStarted = true;
    emit longPressEvent(mPressPoint,filePath);

    //    QLabel *label = new QLabel();
    //    label->setParent(ui->widget);
    //    label->setText(mActionName);
    //    label->setStyleSheet("background-color:#ff0000");
    //    label->setGeometry(mPressPoint.x(),mPressPoint.y(),44,44);
}




