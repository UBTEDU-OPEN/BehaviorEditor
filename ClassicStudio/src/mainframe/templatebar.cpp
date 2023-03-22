#include "templatebar.h"
#include "ui_templatebar.h"
#include <QScrollArea>
#include <QIcon>
#include <QDir>
#include <QToolButton>
#include <QScrollBar>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <thread>
#include <QMenu>
#include<QListWidget>
#include <QDebug>
#include"templateitemdelegate.h"
#include"templatelistviewmodel.h"
#include<QStandardItemModel>
#include<QSize>
#include<QThread>

#define TEMPLATE_BASE_PATH  (qApp->applicationDirPath() + "/resource/libraries/")

TemplateBar::TemplateBar(QWidget *parent):
    QWidget(parent),
    ui(new Ui::TemplateBar),
    tdTemplatePanel(nullptr),
    m_pTemplateData(nullptr){
    ui->setupUi(this);
    init();

}

TemplateBar::~TemplateBar(){
    if(m_pTemplateData){
        delete m_pTemplateData;
        m_pTemplateData = nullptr;
    }
    if(mTemplateItems){
        delete mTemplateItems;
        mTemplateItems = nullptr;
    }
    delete ui;
}

void TemplateBar::init(){
    initVariable();
    initWindow();
    initConnect();
}

void TemplateBar::initVariable(){
    // 加载本地表情和动作数据
    loadLocalTemplateData();
}

void TemplateBar::initWindow(){
    //	ui->label->setStyleSheet(
    //		"font-family: Microsoft Yahei;"
    //        "font-size: 13px;"
    //        "color: rgba(255,255,255,204);"
    //        "padding: 3px 0px 3px 8px;");

    initDockWidget();
    initTemplateList();

}

void TemplateBar::initConnect(){
    connect(tdTemplatePanel, &TemplateDock::sigAddCurFrame, this, &TemplateBar::sigAddCurFrame);
}

void TemplateBar::adjustDockWidget(){
    tdTemplatePanel->resize(300, this->height());
    //	QPoint pt = ui->label->geometry().topRight();
    //	pt = mapToGlobal(pt);
    //	pt.setX(pt.x() + 5);
    //	pt.setY(pt.y() - 5);
    //	tdTemplatePanel->move(pt);
}



void TemplateBar::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    adjustDockWidget();
}

void TemplateBar::initDockWidget(){
    if(tdTemplatePanel == nullptr){
        tdTemplatePanel = new TemplateDock(m_pTemplateData, this);
    }
    tdTemplatePanel->setFeatures(QDockWidget::NoDockWidgetFeatures);
    tdTemplatePanel->setAllowedAreas(Qt::LeftDockWidgetArea);
    tdTemplatePanel->setFloating(true);
    tdTemplatePanel->setVisible(false);

    QWidget *tempWidget = new QWidget();
    tdTemplatePanel->setTitleBarWidget(tempWidget);
    delete tempWidget;
}

void TemplateBar::initTemplateList(){
    // 样式库侧栏底色
    this->setStyleSheet("background-color: #2C313D; border:none;");
    QString nameLabelStyle = "color: rgba( 255, 255, 255, 204);"
                             "font-family: Microsoft Yahei;"
                             "font-size: 12px;"
                             "background-color:transparent;"
                             "padding: 10px 0 10px 8px;";
    mTemplateItems = new QList<TemplateItem*>();
    if(m_pTemplateData){
        QList<BaseData*> baseList;
        foreach(TypeData * pTypeData, m_pTemplateData->get()){
            qDebug() << "typeName===" << pTypeData->getName();
            foreach(ItemData * pItemData, pTypeData->get()){
                foreach (BaseData *baseData, pItemData->get()) {
                    qDebug() << "actionName=====" << baseData->getName();
                    baseList.push_back(baseData);
                }
            }
        }
        for (int i = 0; i < baseList.size(); ++i) {
            BaseData *baseData =  baseList.at(i);
            TemplateItem *itemWidget = new TemplateItem(baseData->getName(),this);
            itemWidget->setFiles(baseData->getFiles());
            itemWidget->setIndex(i);
            QListWidgetItem *item = new QListWidgetItem();

            if(i%2 == 0 ){
                itemWidget->setStyleSheet("background-color:#373C49");
            }else {
                itemWidget->setStyleSheet("background-color:#252A35");
            }
            connect(itemWidget,&TemplateItem::sigAddCurFrame,this,&TemplateBar::sigAddCurFrame);
            connect(itemWidget,&TemplateItem::selectedItem,this,[=](int index){
                for(QList<TemplateItem*>::iterator it = mTemplateItems->begin(); it != mTemplateItems->end();it++){

                    TemplateItem* itemW = *it;
                    if(itemW->getIndex() == index){
                        itemW->setSelected(true);
                    }else {
                        itemW->setSelected(false);
                    }

                }
            });
            connect(itemWidget,&TemplateItem::longPressEvent,this,&TemplateBar::sigItemLongPress);
            connect(itemWidget,&TemplateItem::longPressReleaseEvent,this,&TemplateBar::sigItemLongPressRelease);
            connect(itemWidget,&TemplateItem::longPressMoveEvent,this,&TemplateBar::sigItemLongPressMove);
            QSize size = item->sizeHint();
            item->setSizeHint(QSize(size.width(), 44));
            ui->listWidget->addItem(item);
            itemWidget->setSizeIncrement(size.width(), 44);
            ui->listWidget->setItemWidget(item, itemWidget);
            mTemplateItems->push_back(itemWidget);
        }
    }
}

bool TemplateBar::loadLocalTemplateData(){
    QDir dir(TEMPLATE_BASE_PATH);
    if(!dir.exists()) return false;

    if(m_pTemplateData){
        delete m_pTemplateData;
        m_pTemplateData = nullptr;
    }
    m_pTemplateData = new TemplateData;

    QFileInfoList typeFiles = dir.entryInfoList(QDir::Dirs);
    foreach(const QFileInfo &typeFile, typeFiles){
        QString typeFileName = typeFile.fileName();
        if(typeFileName == "." || typeFileName == "..") continue;

        TypeData *pTypeData = new TypeData;
        pTypeData->setName(typeFileName);
        m_pTemplateData->add(pTypeData);

        QFileInfoList itemFiles = QDir(typeFile.filePath()).entryInfoList(QDir::Dirs);
        foreach(const QFileInfo &itemFile, itemFiles){
            QString itemFileName = itemFile.fileName();
            if(itemFileName == "." || itemFileName == "..") continue;

            ItemData *pItemData = new ItemData;
            pItemData->setName(itemFileName);
            pTypeData->add(pItemData);

            QFileInfoList baseFiles = QDir(itemFile.filePath()).entryInfoList(QDir::Dirs | QDir::Files, QDir::DirsLast);
            foreach(const QFileInfo &baseFile, baseFiles){
                QString baseFileName = baseFile.fileName();
                QString baseFilePath = baseFile.filePath();
                if(baseFileName == "." || baseFileName == "..") continue;

                if(baseFile.isFile() && baseFileName.endsWith("icon_normal.png")){
                    pItemData->setNormalIcon(baseFilePath);
                } else if(baseFile.isFile() && baseFileName.endsWith("icon_hovering.png")){
                    pItemData->setHoveringIcon(baseFilePath);
                } else if(baseFile.isFile() && baseFileName.endsWith("icon_selected.png")){
                    pItemData->setSelectedIcon(baseFilePath);
                } else {
                    BaseData *pBaseData = new BaseData;
                    pBaseData->setName(baseFileName);
                    pItemData->add(pBaseData);

                    QFileInfoList files = QDir(baseFilePath).entryInfoList(QDir::Files);
                    foreach(const QFileInfo &file, files){
                        QString filePath = file.filePath();
                        qDebug() << "filePath====" << filePath;
                        if(filePath.endsWith("logo.png")){
                            if(pBaseData->getLogo().isEmpty()){
                                pBaseData->setLogo(filePath);
                            }
                        } else {
                            pBaseData->add(filePath);
                        }
                    }
                }
            }
        }
    }

    return true;
}

void TemplateBar::selectTemplateType(TemplateButton &tbSelected){
    tdTemplatePanel->setVisible(true);

    // 改变其它按钮的样式
    for(TemplateButton &tbCurrent: this->listTemplateButtons){
        if(tbCurrent.getNumber() != tbSelected.getNumber()) tbCurrent.unselect();
    }

    if(this->m_pTemplateData){
        // 调整位置
        adjustDockWidget();

        // 重复点击则切换显隐
        if(this->lastSelectedItemNumber == tbSelected.getNumber()) this->tdTemplatePanel->setVisibility(!this->tdTemplatePanel->getVisibility());
        else this->tdTemplatePanel->setVisibility(true);
        this->lastSelectedItemNumber = tbSelected.getNumber();

        // 滚动条
        QScrollArea *pScrollArea = qobject_cast<QScrollArea*>(tdTemplatePanel->widget());
        if(pScrollArea){
            QPoint point = tbSelected.pos();
            pScrollArea->verticalScrollBar()->setSliderPosition(point.y());
        }
    }

    // 调整弹出层的内容
    this->tdTemplatePanel->showPage(tbSelected.getName());
}

void TemplateBar::addItem(const QString& filePath)
{
    const auto typeList = m_pTemplateData->get();
    for(auto dataType : typeList) {
        if(dataType->getName().toUtf8() == QString::fromLocal8Bit("动作").toUtf8()) {
            BaseData *pBaseData = new BaseData;
            QFileInfo info(filePath);
            pBaseData->setName(info.baseName());
            pBaseData->add(filePath);

            TemplateItem *itemWidget = new TemplateItem(pBaseData->getName(),this);
            itemWidget->setFiles(pBaseData->getFiles());
            int sum = 0;
            auto itemDataList = dataType->get();
            for(auto itemData : itemDataList) {
                sum += itemData->get().size();
                qDebug() << "itemData size=" << itemData->get().size();
                if(itemData->getName().toUtf8() == QString::fromLocal8Bit("自定义").toUtf8()) {
                    itemData->add(pBaseData);
                }
            }
            qDebug() << "sum=" << sum;
            int i = sum + 1;
            itemWidget->setIndex(i);
            QListWidgetItem *item = new QListWidgetItem();

            if((i+1)%2 == 0 ){
                itemWidget->setStyleSheet("background-color:#373C49");
            }else {
                itemWidget->setStyleSheet("background-color:#252A35");
            }
            connect(itemWidget,&TemplateItem::longPressEvent,this,&TemplateBar::sigItemLongPress);
            connect(itemWidget,&TemplateItem::longPressReleaseEvent,this,&TemplateBar::sigItemLongPressRelease);
            connect(itemWidget,&TemplateItem::longPressMoveEvent,this,&TemplateBar::sigItemLongPressMove);
            connect(itemWidget,&TemplateItem::sigAddCurFrame,this,&TemplateBar::sigAddCurFrame);
            connect(itemWidget,&TemplateItem::selectedItem,this,[=](int index){
                for(QList<TemplateItem*>::iterator it = mTemplateItems->begin(); it != mTemplateItems->end();it++){

                    TemplateItem* itemW = *it;
                    if(itemW->getIndex() == index){
                        itemW->setSelected(true);
                    }else {
                        itemW->setSelected(false);
                    }

                }
            });
            QSize size = item->sizeHint();
            item->setSizeHint(QSize(size.width(), 44));
            ui->listWidget->addItem(item);
            itemWidget->setSizeIncrement(size.width(), 44);
            ui->listWidget->setItemWidget(item, itemWidget);
            mTemplateItems->push_back(itemWidget);
        }
    }
    update();
}
