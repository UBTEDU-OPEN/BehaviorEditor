#include "robotlistwidget.h"
#include "ui_robotlistwidget.h"

RobotListWidget::RobotListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotListWidget)
{
    ui->setupUi(this);
   // ui->robotListWidget->addItem();
    mInfos = new QList<RobotInfo>();
    mIndex = 0;
}

RobotListWidget::~RobotListWidget()
{
    delete ui;
    delete mInfos;
}

void RobotListWidget::setRobotinfoList(QList<RobotInfo> *robotList)
{
    QList<RobotInfo> diffInfos = diffRobotInfos(robotList);
    int diffSize = diffInfos.size();
    qDebug()<< "diffInfo.size=====" << diffSize;
    if(diffSize != 0){
        for(int i=0;i<diffInfos.size(); i++){

            RobotInfo robotInfo = diffInfos.at(i);
            QWidget *widget = new QWidget(this);
            if(mIndex % 2 == 0){
                widget->setStyleSheet("background-color:#2B3140;");
            }else {
                widget->setStyleSheet("background-color:#313747;");
            }
            QHBoxLayout *hLayout = new QHBoxLayout(widget);
            QLabel *snLabel = new QLabel(widget);
            QString sn = "sn:" + robotInfo.sn;
            snLabel->setText(sn);
            snLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

            snLabel->setStyleSheet("background-color:transparent;");

            QPushButton *connBtn = new QPushButton(widget);
            connBtn->setText(tr("Connect"));
            connBtn->setObjectName(QString::number(mIndex));
            connBtn->setMinimumSize(QSize(68,28));
            connBtn->setMaximumSize(QSize(68,28));
            connBtn->setStyleSheet("QPushButton{color: rgb(255, 255, 255);border-radius:14px 14px 14px 14px;background-color: rgb(0, 165, 255);}"
                                   "QPushButton:hover{background-color: rgb(85, 255, 255);}"
                                   "QPushButton:pressed{background-color: rgb(144, 144, 144);}");

            connect(connBtn,&QPushButton::clicked,[=](){
                int index = connBtn->objectName().toInt();
                qDebug() << "index====" << index;
                emit listConnectionBtnClick(mInfos->at(index));
            });
            hLayout->addWidget(snLabel,3);
            hLayout->addWidget(connBtn,1);

            widget->setLayout(hLayout);
            QListWidgetItem *item = new QListWidgetItem();

            QSize size = item->sizeHint();
            item->setSizeHint(QSize(size.width(), 44));
            ui->robotListWidget->addItem(item);
            widget->setSizeIncrement(size.width(), 44);
            ui->robotListWidget->setItemWidget(item, widget);
            mIndex++;

        }
    }


}

QList<RobotInfo> RobotListWidget::diffRobotInfos(QList<RobotInfo> *robotinfos)
{
    qDebug() << "mInfos.size=====" << mInfos->size();
    if(this->mInfos->size() == 0){
        QList<RobotInfo> diffInfos;
        for(int i=0; i<robotinfos->size();i++){
            diffInfos.push_back(robotinfos->at(i));
            this->mInfos->push_back(robotinfos->at(i));
        }
        return diffInfos;
    }else {
        QList<RobotInfo> diffInfos;
        for(int i=0;i<robotinfos->size();i++){
            RobotInfo info_i = robotinfos->at(i);
            if(!mInfos->contains(info_i)){
               diffInfos.push_back(info_i);
               mInfos->push_back(info_i);
            }
        }
        return diffInfos;
    }
}
