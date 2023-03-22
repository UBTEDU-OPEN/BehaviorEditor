#include "improtitemwidget.h"
#include "ui_improtitemwidget.h"
#include<QDebug>
#include"datacenter.h"

ImprotItemWidget::ImprotItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImprotItemWidget)
{
    ui->setupUi(this);
    connect(DataCenter::getInstance(),&DataCenter::sendFileResult,this,&ImprotItemWidget::sendFileComplate);
}

ImprotItemWidget::~ImprotItemWidget()
{
    if(mTimer != NULL){
        if(mTimer->isActive()){
            mTimer->stop();
        }
        delete mTimer;
        mTimer = NULL;
    }
    delete mFileInfo;
    delete mItem;
    delete ui;
}

void ImprotItemWidget::setItem(QListWidgetItem *item)
{
    this->mItem = item;
}

QListWidgetItem* ImprotItemWidget::getItem()
{
    return mItem;
}

void ImprotItemWidget::setFileInfo(QFileInfo *fileInfo)
{
    this->mFileInfo = fileInfo;
    ui->nameLabel->setText(fileInfo->fileName());
    qint64 size = fileInfo->size();
    QString sizeStr = byteToString(size);
    ui->sizeLabel->setText(sizeStr);


}

QFileInfo* ImprotItemWidget::getFileInfo()
{
    return mFileInfo;
}

bool ImprotItemWidget::isChecked()
{
    return ui->checkBox->isChecked();
}

void ImprotItemWidget::setChecked(bool checked)
{
    ui->checkBox->setChecked(checked);
}

void ImprotItemWidget::startImport()
{
    stopImport();
    mTimer = new QTimer();
    connect(mTimer,&QTimer::timeout,this,&ImprotItemWidget::importTimerOut);
    mTimer->start(400);
    ui->progressBar->setValue(0);
    ui->stateLabel->setText(tr("Improting"));
    auto filePath = mFileInfo->filePath();
    DataCenter::getInstance()->sendFile(filePath);

}

void ImprotItemWidget::stopImport()
{
    if(NULL != mTimer){
        if(mTimer->isActive()){
            mTimer->stop();
            delete mTimer;
            mTimer = NULL;
        }
    }
}

void ImprotItemWidget::importTimerOut()
{
    int progressValue = ui->progressBar->value();
    ui->progressBar->setValue(progressValue + 20);
    if(ui->progressBar->value()>=100){
        ui->progressBar->setValue(99);
        stopImport();
    }
}

void ImprotItemWidget::sendFileComplate(bool result)
{
    qDebug() << "result====" << result;
    stopImport();
    if(result){
        ui->progressBar->setValue(100);
        ui->stateLabel->setText(tr("Export Complate"));
    }else {
        ui->stateLabel->setText(tr("Export Fail"));
    }
    setChecked(false);
    emit importSuccess();

}


QString ImprotItemWidget::byteToString(qint64 size)
{
    int kb = 1024;
    int mb = 1024 * 1024;

    if(size < kb){
        return "1KB";
    }
    if(size >=kb && size < mb){
        float num =(float)size/(float)kb;

        QString str = QString::number(num, 'f', 2);

        return str + "KB";

    }
    if(size >= mb){
        float num =(float)size/(float)mb;

        QString str = QString::number(num, 'f', 2);
        return str + "MB";
    }
    return "";
}
