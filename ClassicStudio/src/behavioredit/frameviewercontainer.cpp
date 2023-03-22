#include "frameviewercontainer.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QCursor>
#include <QtMath>
#include <QDebug>

FrameViewerContainer::FrameViewerContainer(int fn, double fd, int fps, QWidget *parent)
    : QWidget(parent)
    , frameContainer_(nullptr)
    , frameViewer_(nullptr)
    , frameViewerContainerVScrollBar_(nullptr)
{
    setObjectName("FrameViewerContainer");
    setFixedHeight(44*2+96*2+14+20);
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(1);
    frameContainer_ = new QScrollArea;
    frameContainer_->setFixedHeight(44*2+96*2+14+20);
    frameContainer_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(frameContainer_);
    frameViewerContainerVScrollBar_ = new QScrollBar(Qt::Vertical);
    frameViewerContainerVScrollBar_->setEnabled(false);
    frameViewerContainerVScrollBar_->hide();
//    auto verticalLayout = new QVBoxLayout;

    connect(frameViewerContainerVScrollBar_, &QScrollBar::valueChanged, this, &FrameViewerContainer::onVScrollBarValueChanged);

    auto hScrollBar = frameContainer_->horizontalScrollBar();
    connect(hScrollBar, &QScrollBar::valueChanged,
            this, &FrameViewerContainer::onHorizantolScrolled);
    connect(hScrollBar, &QScrollBar::rangeChanged,
            this, &FrameViewerContainer::onHorizantolScrollBarRangeChanged);

    frameViewer_ = new FrameViewer(fn, fd, fps);
    connect(frameViewer_, &FrameViewer::behaviorLayerContainerVScrollBarValueChanged,
            this, &FrameViewerContainer::onBehaviorLayerContainerVScrollBarValueChanged);
    connect(frameViewer_, &FrameViewer::behaviorVScrollBarRangeChanged,
            this, &FrameViewerContainer::onBehaviorVScrollBarRangeChanged);
    connect(frameViewer_, &FrameViewer::currentFrameIndexChanged,
            this, &FrameViewerContainer::currentFrameIndexChanged);
//    connect(frameViewer, &FrameViewer::nextKeyFrame, this, &FrameViewerContainer::nextKeyFrame);
    connect(frameViewer_, &FrameViewer::wheelWhileCtrlPressing,
            this, &FrameViewerContainer::wheelWhileCtrlPressing);
    frameContainer_->setWidget(frameViewer_);
    frameContainer_->setMouseTracking(true);
    frameContainer_->viewport()->setMouseTracking(true);

    connect(this, &FrameViewerContainer::sigKeyEvent, frameViewer_, &FrameViewer::onKeyEvent);
    configStyle();

    connect(frameViewer_,&FrameViewer::zoomIn,this,&FrameViewerContainer::zoomIn);
    connect(frameViewer_,&FrameViewer::zoomOut,this,&FrameViewerContainer::zoomOut);
}

FrameViewerContainer::~FrameViewerContainer()
{
}

void FrameViewerContainer::configStyle()
{
    frameContainer_->horizontalScrollBar()->setFixedHeight(14);
    frameViewerContainerVScrollBar_->setFixedWidth(14);
}

void FrameViewerContainer::onChangeFrameIndex(int frameIndex)
{
    if (frameViewer_) {
        frameViewer_->onChangeFrameIndex(frameIndex);
    }
}

void FrameViewerContainer::setScaleValue(int scale)
{
    if (frameViewer_) {
        frameViewer_->setScaleValue(scale);
    }
}

void FrameViewerContainer::onHorizantolScrolled(int value)
{
//    qDebug() << "FrameViewerContainer::onHorizantolScrolled value" <<value;
    auto hScrollBar = frameContainer_->horizontalScrollBar();
    auto percent = static_cast<double>(value) / (hScrollBar->maximum() - hScrollBar->minimum());
    if (frameViewer_) {
        frameViewer_->onHorizantolScrolled(percent);
    }
}

void FrameViewerContainer::onHorizantolScrollBarRangeChanged(int min, int max)
{
    if (!frameViewer_) {
        return;
    }
    auto maxFrame = frameViewer_->getFrameNumber();
    double frame_dis = frameViewer_->frameDistance();
    auto frameViewerLen = maxFrame * frame_dis;
    auto frameViewerContainerRect = geometry();
    if (frameViewerLen <= frameViewerContainerRect.width()) {
        return;
    }
    auto range = max - min;
    if (range == 0) {
        return;
    }
    int fixedFrame = 0;
    int diff = 0;
    frameViewerContainerRect.moveTo(mapToGlobal(QPoint(0, 0)));
    if (frameViewerContainerRect.contains(QCursor::pos())) {
        fixedFrame = frameViewer_->getFrameOnCursorWhileScaling();
        diff = QCursor::pos().x() - frameViewerContainerRect.x();
//        qDebug() << "FrameViewerContainer::onHorizantolScrollBarRangeChanged cursor in, fixedFrame=" << fixedFrame
//                 << "diff:" << diff
//                 << "frame_dis:" << frame_dis;
    } else {
        fixedFrame = frameViewer_->getCurrentFrameIndex();
        diff = frameViewerContainerRect.width() / 2;
//        qDebug() << "FrameViewerContainer::onHorizantolScrollBarRangeChanged cursor out, fixedFrame=" << fixedFrame
//                 << "diff:" << diff
//                 << "frame_dis:" << frame_dis;
    }
    double leftBorder = fixedFrame * frame_dis  - diff;
    int leftBorderWhenScrollToMaxValue = frameViewerLen - frameViewerContainerRect.width();
    if (leftBorder < 0) {
        leftBorder = 0;
    } else if (leftBorder > leftBorderWhenScrollToMaxValue) {
        leftBorder = leftBorderWhenScrollToMaxValue;
    }
    double percent = leftBorder / (frameViewerLen - frameViewerContainerRect.width());
    auto hScrollBar = frameContainer_->horizontalScrollBar();
    auto newValue = percent * range;
    hScrollBar->setValue(newValue);
}

void FrameViewerContainer::onVScrollBarValueChanged(int value)
{
    if (slideByFrameViewerContainerVScrollBar()) {
        auto percent = static_cast<double>(value) / (frameViewerContainerVScrollBar_->maximum() - frameViewerContainerVScrollBar_->minimum());
        if (frameViewer_) {
            frameViewer_->onFrameViewerContainerVScrollBarValueChanged(percent);
        }
        emit frameViewerContainerVScrollBarValueChanged(percent);
    }
}

void FrameViewerContainer::onBehaviorLayerContainerVScrollBarValueChanged(double value)
{
    if (frameViewerContainerVScrollBar_ && !slideByFrameViewerContainerVScrollBar()) {
        frameViewerContainerVScrollBar_->setValue(value * (frameViewerContainerVScrollBar_->maximum() - frameViewerContainerVScrollBar_->minimum()));
    }
    emit behaviorLayerContainerVScrollBarValueChanged(value);
}

void FrameViewerContainer::onBehaviorVScrollBarRangeChanged(int min, int max)
{
    frameViewerContainerVScrollBar_->setMinimum(min);
    frameViewerContainerVScrollBar_->setMaximum(max);
}

void FrameViewerContainer::addFileToCurrentFrame(const QString &filePath)
{
    if (frameViewer_) {
        frameViewer_->addFileToCurrentFrame(filePath);
    }
}

void FrameViewerContainer::setCurrentFrameIndex(int fi)
{
    if (frameViewer_) {
        frameViewer_->setCurrentFrameIndex(fi);
    }
}

QRect FrameViewerContainer::getBehaviorLayerRect()
{
    return frameViewer_->getBehaviorLayerRect();
}

void FrameViewerContainer::resizeEvent(QResizeEvent * /*e*/)
{
    auto hScrollBar = frameContainer_->horizontalScrollBar();
    frameViewer_->setFixedHeight(frameContainer_->height() - hScrollBar->height());
}

void FrameViewerContainer::keyPressEvent(QKeyEvent* e)
{
    emit sigKeyEvent(e);
}

bool FrameViewerContainer::slideByFrameViewerContainerVScrollBar() const
{
    auto frameViewerContainerVScrollBarGeometry = frameViewerContainerVScrollBar_->geometry();
    frameViewerContainerVScrollBarGeometry.moveTo(frameViewerContainerVScrollBar_->mapToGlobal(QPoint(0, 0)));
    return frameViewerContainerVScrollBarGeometry.contains(QCursor::pos()) || frameViewerContainerVScrollBar_->isSliderDown();
}

void FrameViewerContainer::addMusic(const QString& filePath)
{
    if (frameViewer_) {
        frameViewer_->addMusic(filePath);
    }
}

void FrameViewerContainer::onFrameDivisionChanged(int frameDivision)
{
    frameViewer_->onFrameDivisionChanged(frameDivision);
}
