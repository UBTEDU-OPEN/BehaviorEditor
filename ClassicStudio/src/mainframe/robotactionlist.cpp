#include "robotactionlist.h"
#include "ui_robotactionlist.h"

#include "datacenter.h"
#include<QToolButton>
#include<QDebug>
#include<importdialog.h>
#include<robotactionlistitem.h>

RobotActionList::RobotActionList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RobotActionList)
{
    ui->setupUi(this);


    connect(ui->stopBtn,&QToolButton::clicked,[this]{
        qDebug() << "stopActionBtn=======";
        if(!triggeredItemName_.isEmpty()) {
            DataCenter::getInstance()->stopRobotAction(triggeredItemName_);
        }
    });

    connect(ui->importBtn,&QToolButton::clicked,[this]{
        qDebug() << "importActionBtn=======";
        ImportDialog *importDialog = new ImportDialog(this);
        importDialog->exec();
    });

    connect(ui->refreshBtn,&QToolButton::clicked,[]{
        qDebug() << "refreshListBtn=======";
        DataCenter::getInstance()->refreshActionList();
    });

    connect(ui->deleteBtn,&QToolButton::clicked,[this]{
        qDebug() << "deleteItemBtn=======" << selectedItemName_;
        if(!selectedItemName_.isEmpty()) {
            RobotActionListItem * selectedItem = NULL;
            for(QList<RobotActionListItem *>::iterator it = actionList_->begin();it!=actionList_->end(); it++) {
                RobotActionListItem * actionItem = *it;
                if(actionItem->getActionName() == selectedItemName_) {               
                    selectedItem = actionItem;
                    break;
                }

            }
            if(selectedItem != NULL){
                QListWidgetItem *item = selectedItem->getItem();
                ui->listWidget->removeItemWidget(item);
                actionList_->removeOne(selectedItem);
                delete selectedItem;
                selectedItem = NULL;
            }

            DataCenter::getInstance()->deleteActionFile(selectedItemName_);
        }
    });
    connect(DataCenter::getInstance(),&DataCenter::robotConnectionStateChanged,this,&RobotActionList::robotConnectionStateChanged);

    ui->listWidget->setFrameShape(QListWidget::NoFrame);
    ui->listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    setBtnDisable(true);
    this->setStyleSheet("background-color: #2C313D; border:none;");
}

RobotActionList::~RobotActionList()
{
    delete ui;
}

void RobotActionList::clearList()
{
    qDebug() << "clearList=====";
    if(NULL != actionList_){
        for(QList<RobotActionListItem *>::iterator it = actionList_->begin();it!=actionList_->end(); it++) {
            RobotActionListItem * actionItem = *it;
            ui->listWidget->removeItemWidget(actionItem->getItem());
            delete actionItem;
            actionItem = NULL;
        }
        actionList_->clear();
    }else {
        actionList_ = new QList<RobotActionListItem*>();
    }

}

void RobotActionList::createNewList(const QList<QString>& actionNames)
{
    clearList();

    int i = 0;
    for(auto actionName : actionNames) {
        RobotActionListItem * itemWidget = new RobotActionListItem(this);
        QListWidgetItem *item = new QListWidgetItem();
        itemWidget->setActionName(actionName);
        itemWidget->setItem(item);
        actionList_->push_back(itemWidget);
        if(i%2 == 0 ){
            itemWidget->setStyleSheet("background-color:#373C49");
        }else {
            itemWidget->setStyleSheet("background-color:#252A35");
        }
        connect(itemWidget,&RobotActionListItem::itemTriggered,this,[=](const QString& itemName){
            triggeredItemName_ = itemName;
            DataCenter::getInstance()->playRobotAction(itemName);
        });
        connect(itemWidget,&RobotActionListItem::itemSelected,this,[=](const QString& itemName){
            selectedItemName_ = itemName;
            for(QList<RobotActionListItem *>::iterator it = actionList_->begin();it!=actionList_->end(); it++) {
                RobotActionListItem * actionItem = *it;
                if(actionItem->getActionName() == itemName) {
                    actionItem->setSelcetStyle(true);
                } else {
                    actionItem->setSelcetStyle(false);
                }
            }
        });
        QSize size = item->sizeHint();
        item->setSizeHint(QSize(size.width(), 44));
        ui->listWidget->addItem(item);
        itemWidget->setSizeIncrement(size.width(), 44);
        ui->listWidget->setItemWidget(item, itemWidget);
        i++;
    }




}

void RobotActionList::robotConnectionStateChanged(int state, QString sn)
{
    if(state == 2){
        setBtnDisable(false);
    }else {
        setBtnDisable(true);
        clearList();
    }
}

void RobotActionList::setBtnDisable(bool isDisable)
{
    ui->stopBtn->setDisabled(isDisable);
    ui->importBtn->setDisabled(isDisable);
    ui->refreshBtn->setDisabled(isDisable);
    ui->deleteBtn->setDisabled(isDisable);
//    ui->exportBtn->setDisabled(isDisable);
    if(isDisable){
        ui->stopBtn->setIcon(QIcon(":/image/res/image/template/ic_stop_disable.png"));
        ui->importBtn->setIcon(QIcon(":/image/res/image/template/ic_import_disable.png"));
        ui->refreshBtn->setIcon(QIcon(":/image/res/image/template/ic_refreshic_disable.png"));
        ui->deleteBtn->setIcon(QIcon(":/image/res/image/template/ic_delete_disable.png"));
//        ui->exportBtn->setIcon(QIcon(":/image/res/image/template/ic_download_disable.png"));
    }else {
        ui->stopBtn->setIcon(QIcon(":/image/res/image/template/ic_stop.png"));
        ui->importBtn->setIcon(QIcon(":/image/res/image/template/ic_import.png"));
        ui->refreshBtn->setIcon(QIcon(":/image/res/image/template/ic_refresh.png"));
        ui->deleteBtn->setIcon(QIcon(":/image/res/image/template/ic_delete.png"));
//        ui->exportBtn->setIcon(QIcon(":/image/res/image/template/ic_download.png"));
    }
}
