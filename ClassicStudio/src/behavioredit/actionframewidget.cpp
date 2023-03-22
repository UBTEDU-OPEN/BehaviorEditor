#include "actionframewidget.h"
#include "ui_actionframewidget.h"

#include <QPaintEvent>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QFile>
#include <QDebug>

#include "datacenter.h"

ActionFrameWidget::ActionFrameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActionFrameWidget)
{
    ui->setupUi(this);
    setFixedSize(14,63);
    setProperty("selected",false);
    QFile ssfile(":/res/qss/actionFrameWidget.qss");
    if(ssfile.open(QFile::ReadOnly)) {
        setStyleSheet(ssfile.readAll());
    }
}

ActionFrameWidget::~ActionFrameWidget()
{
    delete ui;
}

void ActionFrameWidget::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QWidget::paintEvent(event);
}

void ActionFrameWidget::setFrameSelected(bool selected)
{
    selected_ = selected;
    setProperty("selected",selected);
    style()->unpolish(this);
    style()->polish(this);
}

void ActionFrameWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
    DataCenter::getInstance()->previewFrameData(frameIndex_);
}
