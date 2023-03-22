#include "importdialog.h"
#include "ui_importdialog.h"

#include<QFileDialog>
#include<QDebug>
#include"datacenter.h"
#include<QListIterator>
#include"improtcanceldialog.h"

ImportDialog::ImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)

{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(true);
    mItemWidgets = new QList<ImprotItemWidget*>();
    ui->listWidget->setFrameShape(QListWidget::NoFrame);
    ui->listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setSpacing(5);
    //add current
    auto filePath = UBTDataCenter->compressCurrentBehavior();
    if(!filePath.isEmpty()) {
        ImprotItemWidget *itemWidget = new ImprotItemWidget(this);
        QListWidgetItem *item = new QListWidgetItem();
        QFileInfo *fileInfo = new QFileInfo(filePath);
        QSize size = item->sizeHint();
        item->setSizeHint(QSize(size.width(), 44));
        ui->listWidget->addItem(item);
        itemWidget->setSizeIncrement(size.width(), 44);
        ui->listWidget->setItemWidget(item, itemWidget);
        itemWidget->setItem(item);
        itemWidget->setFileInfo(fileInfo);
        mItemWidgets->push_back(itemWidget);
        itemWidget->setChecked(true);
    }

}

ImportDialog::~ImportDialog()
{
    if(!mItemWidgets->isEmpty()){
        for(QList<ImprotItemWidget *>::iterator it = mItemWidgets->begin(); it != mItemWidgets->end(); it++){
            ImprotItemWidget *itemWidget = *it;
            delete itemWidget;
        }
    }
    delete mItemWidgets;
    delete ui;
}

void ImportDialog::on_closeBtn_clicked()
{
    if(NULL != mCheckedItems && mCheckedItems->size() != 0 && isStartImport){
        ImprotCancelDialog *cancelDialog = new ImprotCancelDialog(this);
        connect(cancelDialog,&ImprotCancelDialog::cancelImprot,this,&ImportDialog::onCancelImprot);
        cancelDialog->exec();
    }else {
        this->close();
    }
}

void ImportDialog::on_addBtn_clicked()
{
    auto filePath = QFileDialog::getOpenFileName(this, "Open", qApp->applicationDirPath(), "Behavior Files(*.xml *.hts *.mp3 *.zip *.rar)");
    if(filePath.isEmpty()){
        qDebug() << "Open file path is empty.";
        return;
    }
    if(!mItemWidgets->isEmpty()){
        for(QList<ImprotItemWidget *>::iterator it = mItemWidgets->begin(); it != mItemWidgets->end(); it++){
            ImprotItemWidget *itemWidget = *it;
            QFileInfo *info = itemWidget->getFileInfo();
            auto path = info->filePath();
            if(filePath == path){
                return;
            }
        }
    }
    ImprotItemWidget *itemWidget = new ImprotItemWidget(this);
    QListWidgetItem *item = new QListWidgetItem();
    QFileInfo *fileInfo = new QFileInfo(filePath);
    QSize size = item->sizeHint();
    item->setSizeHint(QSize(size.width(), 44));
    ui->listWidget->addItem(item);
    itemWidget->setSizeIncrement(size.width(), 44);
    ui->listWidget->setItemWidget(item, itemWidget);
    itemWidget->setItem(item);
    itemWidget->setFileInfo(fileInfo);
    mItemWidgets->push_back(itemWidget);
    bool isAllChecked = ui->checkBox->isChecked();
    itemWidget->setChecked(isAllChecked);

}

void ImportDialog::on_deleteBtn_clicked()
{
    QList<ImprotItemWidget *> *checkedItems = new QList<ImprotItemWidget*>();
    for (int i = 0;  i < mItemWidgets->size(); i++) {
        ImprotItemWidget *itemWidget = mItemWidgets->at(i);
        if(itemWidget->isChecked()){
            checkedItems->push_back(itemWidget);
        }
    }
    int checkedSize = checkedItems->size();
    if(checkedSize > 0){
        for(int i=0; i<checkedSize; i++){
            ImprotItemWidget * checkedItem = checkedItems->at(i);
            QListWidgetItem* item = checkedItem->getItem();
            ui->listWidget->removeItemWidget(item);
            mItemWidgets->removeOne(checkedItem);
            delete checkedItem;
        }
        delete checkedItems;
    }
}

void ImportDialog::on_checkBox_toggled(bool checked)
{
    if(!mItemWidgets->isEmpty()){
        for(QList<ImprotItemWidget *>::iterator it = mItemWidgets->begin(); it != mItemWidgets->end(); it++){
            ImprotItemWidget *itemWidget = *it;
            itemWidget->setChecked(checked);
        }
    }
}

void ImportDialog::on_exportBtn_clicked()
{
    mCheckedItems = new QList<ImprotItemWidget*>();
    for (int i = 0;  i < mItemWidgets->size(); i++) {
        ImprotItemWidget *itemWidget = mItemWidgets->at(i);
        if(itemWidget->isChecked()){
            mCheckedItems->push_back(itemWidget);
        }
    }
    int checkedSize = mCheckedItems->size();
    if(checkedSize > 0){
        isStartImport = true;
        setButtonStartStyle(isStartImport);
        startImport();
    }
}

void ImportDialog::startImport()
{
    ImprotItemWidget * checkedItem = mCheckedItems->first();
    checkedItem->startImport();
    connect(checkedItem,&ImprotItemWidget::importSuccess,this,[=](){
//        QListWidgetItem* item = checkedItem->getItem();
//        ui->listWidget->removeItemWidget(item);
//        mItemWidgets->removeOne(checkedItem);
        if(!mCheckedItems->isEmpty()){
            mCheckedItems->removeFirst();
            if(mCheckedItems->size()==0){
                isStartImport = false;
                setButtonStartStyle(isStartImport);
            }else {
                if(isStartImport){
                    startImport();
                }
            }
        }
    });
}

void ImportDialog::setButtonStartStyle(bool isStart)
{

     ui->deleteBtn->setDisabled(isStart);
     ui->addBtn->setDisabled(isStart);
     ui->exportBtn->setDisabled(isStart);

}

void ImportDialog::on_cancelBtn_clicked()
{
    if(NULL != mCheckedItems){
        if(mCheckedItems->size() != 0){
            if(isStartImport){
                ImprotCancelDialog *cancelDialog = new ImprotCancelDialog(this);
                connect(cancelDialog,&ImprotCancelDialog::cancelImprot,this,&ImportDialog::onCancelImprot);
                cancelDialog->exec();
            }else {
                this->close();
            }
        }else {
            this->close();
        }
    }else {
        this->close();
    }
}

void ImportDialog::onCancelImprot()
{
    isStartImport = false;
    if(NULL != mCheckedItems){
        if(mCheckedItems->size() > 0){
            for(QList<ImprotItemWidget *>::iterator it = mCheckedItems->begin();it != mCheckedItems->end();it++){
                ImprotItemWidget *itemWidget = *it;
                itemWidget->stopImport();
            }
        }
    }
}
