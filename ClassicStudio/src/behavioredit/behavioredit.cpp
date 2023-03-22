#include "behavioredit.h"

#include <QPushButton>
#include <QFile>
#include <QTextCodec>
#include <QFileDialog>
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "datacenter.h"

const QString BehaviorEdit::kQssPath{":/res/qss/classicbehavioredit.qss"};

BehaviorEdit::BehaviorEdit(QWidget *parent)
    : QWidget(parent)
    , mainLayout_(nullptr)
    , controlWidget_(nullptr)
{
    setDefaultStyle();

    mainLayout_ = new QHBoxLayout(this);
    setLayout(mainLayout_);
    controlWidget_ = new ControlWidget(this);
    mainLayout_->addWidget(controlWidget_);
    frameViewerContainer_ = new FrameViewerContainer(2000,50,5);
    mainLayout_->addWidget(frameViewerContainer_);

    connect(controlWidget_, &ControlWidget::scaleValueChanged,
            frameViewerContainer_, &FrameViewerContainer::setScaleValue);
    connect(frameViewerContainer_,&FrameViewerContainer::zoomIn,this,&BehaviorEdit::onZoomIn);
    connect(frameViewerContainer_,&FrameViewerContainer::zoomOut,this,&BehaviorEdit::onZoomOut);
}

BehaviorEdit::~BehaviorEdit()
{
}

QRect BehaviorEdit::getBehaviorLayerRect()
{

    return frameViewerContainer_->getBehaviorLayerRect();
}

void BehaviorEdit::onChangeFrameIndex(int frameIndex)
{
    if (frameViewerContainer_) {
        frameViewerContainer_->onChangeFrameIndex(frameIndex);
    }
}

void BehaviorEdit::addFileToCurrentFrame(const QString &filePath)
{
    if (frameViewerContainer_) {
        frameViewerContainer_->addFileToCurrentFrame(filePath);
    }
}

void BehaviorEdit::setDefaultStyle()
{
    QFile styleSheet(kQssPath);
    if (styleSheet.open(QIODevice::ReadOnly)) {
        setStyleSheet(styleSheet.readAll());
    }
}

void BehaviorEdit::addMusic(const QString& filePath)
{
    frameViewerContainer_->addMusic(filePath);
}

void BehaviorEdit::onFrameDivisionChanged(int frameDivision)
{
    frameViewerContainer_->onFrameDivisionChanged(frameDivision);
}

void BehaviorEdit::onTotalTimeChanged(int secs)
{
    controlWidget_->onDurationChanged(secs);
}

void BehaviorEdit::onZoomIn()
{
    controlWidget_->onZoomIn();
}

void BehaviorEdit::onZoomOut()
{
    controlWidget_->onZoomOut();
}
