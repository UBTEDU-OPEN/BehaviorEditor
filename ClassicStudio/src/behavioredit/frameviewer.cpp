#include "frameviewer.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMediaPlayer>
#include <QDebug>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>

#include "datacenter.h"
#include "frameviewplaceholder.h"

FrameViewer::FrameViewer(int fn, double fd, int fps, QWidget *parent)
    : QWidget(parent)
    , frameNum_(fn)
    , frameDistanceBase_(fd)
    , frameRate_(fps)
    , scaleFactor_(1)
    , currentFrameIndex_(1)
    , frameOnCursorWhileScaling_(0)
{
    setObjectName("FrameViewer");
    auto frameViewerLayout = new QVBoxLayout(this);
    frameViewerLayout->setMargin(0);
    frameViewerLayout->setSpacing(1);

    frameAxis_ = new Axis(Axis::Top, 0, frameNum_, 150, 1, frameDistance(), "%1f");
    frameAxis_->setFixedHeight(41);
    frameAxis_->setObjectName("frameAxis");
    frameViewerLayout->addWidget(frameAxis_);

    FrameViewPlaceHolder *placeHolder = new FrameViewPlaceHolder(this);

    placeHolder->setFixedHeight(14);
    placeHolder->setObjectName("frameViewPlaceHolder");
    frameViewerLayout->addWidget(placeHolder);

    behaviorLayerContainerScrollArea_ = new QScrollArea;
    behaviorLayerContainerScrollArea_->setObjectName("behaviorLayerContainer");
    behaviorLayerContainerScrollArea_->setFixedHeight(30 * 9);
    frameViewerLayout->addWidget(behaviorLayerContainerScrollArea_);

    behaviorLayerContainerScrollArea_->installEventFilter(this);
    behaviorLayerContainerScrollArea_->viewport()->installEventFilter(this);

    behaviorLayerContainer_ = new BehaviorLayerContainer(fn, fd);
    behaviorLayerContainer_->installEventFilter(this);
    behaviorLayerContainerScrollArea_->setWidget(behaviorLayerContainer_);
    behaviorLayerContainerScrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    behaviorLayerContainerScrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto behaviorLayerContainerVScrollBar = behaviorLayerContainerScrollArea_->verticalScrollBar();
    connect(behaviorLayerContainerVScrollBar, &QScrollBar::valueChanged, this, &FrameViewer::onBehaviorLayerContainerVScrollBarValueChanged);
    connect(behaviorLayerContainerVScrollBar, &QScrollBar::rangeChanged, this, &FrameViewer::onBehaviorVScrollBarRangeChanged);
    connect(behaviorLayerContainer_, &BehaviorLayerContainer::changeFramePointer,this, &FrameViewer::setCurrentFrameIndex);
    timeAxis_ = new Axis(Axis::Bottom, 0, 300, 150, 1, 50, "%1s");
    timeAxis_->setFixedHeight(50);
    timeAxis_->setObjectName("timeAxis");
    frameViewerLayout->addWidget(timeAxis_);

//    currentFramePointer_ = new FramePointer(this);
//    currentFramePointer_->hide();
    framePointerTop_ = new FramePointerTop(this);
    framePointerLine_ = new QLabel(this);
    framePointerLine_->setStyleSheet("background: #00a5ff;"); //
    framePointerLine_->setFixedWidth(1);
    framePointerLine_->setVisible(true);
    framePointerBottom_ = new QLabel(this);
    framePointerBottom_->setObjectName("framePointerBottom");
    framePointerTop_->show();
    framePointerBottom_->show();
    framePointerBottom_->setFixedSize(32,16);
    framePointerBottom_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

//    connect(frameAxis_, &Axis::setCurrentFramePos,
//            this, &FrameViewer::setCurrentFramePosAndSendToRobot);
    connect(frameAxis_,&Axis::setCurrentFramePos,
            this,&FrameViewer::setCurrentFramePos);
//    connect(currentFramePointer_,&FramePointer::frameDoubleClicked,this,&FrameViewer::onFrameDoubleClicked);

    setStyleSheet("QWidget#FrameViewer{ background: #252A35; }");
    setCurrentFrameIndex(1);

    connect(behaviorLayerContainer_,&BehaviorLayerContainer::zoomIn,this,&FrameViewer::zoomIn);
    connect(behaviorLayerContainer_,&BehaviorLayerContainer::zoomOut,this,&FrameViewer::zoomOut);

    connect(UBTDataCenter,&DataCenter::newBehavior,this,&FrameViewer::onNewBehavior);

}

FrameViewer::~FrameViewer()
{
}

bool FrameViewer::onNewBehavior()
{
    if (behaviorLayerContainer_) {
        frameRate_ = DataCenter::getInstance()->frameRate();
        double topTickInterval = frameAxis_->getTickInterval();
        double topTickDistance = frameAxis_->tickDistance();
        double bottomTickDistance = topTickDistance * 5;
        double bottomTickInterval = (topTickInterval * bottomTickDistance) / (topTickDistance * frameRate_);
        if (timeAxis_) {
            timeAxis_->setFixedWidth(frameAxis_->width());
            timeAxis_->changeTick(bottomTickInterval, bottomTickDistance);
        }
        setCurrentFrameIndex(1);
        return behaviorLayerContainer_->onNewBehavior();
    }
    return true;
}

void FrameViewer::onChangeFrameIndex(int frameIndex)
{
    setCurrentFrameIndex(frameIndex);
}


void FrameViewer::setScaleValue(int scale)
{
    scaleFactor_ = static_cast<double>(scale) / 10;
    int newWidth = 0;
    if (frameAxis_) {
        frameAxis_->setScaleFactor(scaleFactor_);
        newWidth = frameAxis_->width();
    }
    setFixedWidth(newWidth);
    if (timeAxis_) {
        timeAxis_->setScaleFactor(scaleFactor_);
    }
    if (behaviorLayerContainer_) {
        behaviorLayerContainer_->setScaleFactor(scaleFactor_);
    }
    moveFramePointer();
}

void FrameViewer::onHorizantolScrolled(double value)
{
    if (behaviorLayerContainerScrollArea_) {
        auto frameViewerLayerContainerScrollBar = behaviorLayerContainerScrollArea_->horizontalScrollBar();
        if (frameViewerLayerContainerScrollBar) {
            auto range = static_cast<double>(frameViewerLayerContainerScrollBar->maximum()) - frameViewerLayerContainerScrollBar->minimum();
            auto newValue = range * value;
            frameViewerLayerContainerScrollBar->setValue(newValue);
        }
    }
}

void FrameViewer::onFrameViewerContainerVScrollBarValueChanged(double value)
{
    if (behaviorLayerContainerScrollArea_) {
        behaviorLayerContainerScrollArea_->verticalScrollBar()->setValue(value * (behaviorLayerContainerScrollArea_->verticalScrollBar()->maximum() - behaviorLayerContainerScrollArea_->verticalScrollBar()->minimum()));
    }
}

void FrameViewer::onBehaviorLayerContainerVScrollBarValueChanged(int value)
{
    auto behaviorLayerContainerGeometry = behaviorLayerContainerScrollArea_->geometry();
    behaviorLayerContainerGeometry.moveTo(behaviorLayerContainerScrollArea_->mapToGlobal(QPoint(0, 0)));
    if (behaviorLayerContainerGeometry.contains(QCursor::pos())) {
        auto vscrollbar = behaviorLayerContainerScrollArea_->verticalScrollBar();
        auto range = vscrollbar->maximum() - vscrollbar->minimum();
        if (range != 0) {
            auto percent = static_cast<double>(value) / range;
            emit behaviorLayerContainerVScrollBarValueChanged(percent);
        }
    }
}

void FrameViewer::onBehaviorVScrollBarRangeChanged(int min, int max)
{
    emit behaviorVScrollBarRangeChanged(min, max);
}

void FrameViewer::onActLayerTreeScrolled(double v)
{
    auto behaviorLayerContainerVScrollBar = behaviorLayerContainerScrollArea_->verticalScrollBar();
    auto range = behaviorLayerContainerVScrollBar->maximum() - behaviorLayerContainerVScrollBar->minimum();
    behaviorLayerContainerVScrollBar->setValue(range * v);
}

void FrameViewer::onKeyEvent(QKeyEvent *e)
{
    if (behaviorLayerContainer_) {
        behaviorLayerContainer_->onKeyEvent(e);
    }
}

void FrameViewer::setCurrentFrameIndex(int fi)
{
//    if (currentFramePointer_) {
//        currentFramePointer_->setVisible(true);
//        currentFramePointer_->setFrameIndex(fi);
//        moveFramePointer();
////        currentFramePointer_->move(QPoint(fi * frameDistance() - currentFramePointer_->width() / 2, 0));
////        framePointerLine_->move(fi * frameDistance(),currentFramePointer_->height());
//        if (fi >= frameNum_) {
//            setFrameNumber(frameNum_ + 200);
//        }
//    }

    framePointerTop_->setFrameIndex(fi);
    moveFramePointer();
    if (fi >= frameNum_) {
        setFrameNumber(frameNum_ + 200);
    }

}

int FrameViewer::getCurrentFrameIndex() const
{
    if (framePointerTop_) {
        return framePointerTop_->getFrameIndex();
    }
    return 0;
}

void FrameViewer::setCurrentFramePos(double pos)
{
    auto frame_distance = frameDistance();
    int idx = qRound(pos / frame_distance);
    if (idx < 0) {
        idx = 0;
    }
    DataCenter::getInstance()->setCurrentFrameIndex(idx);
}

void FrameViewer::setCurrentFramePosAndSendToRobot(double pos)
{
    setCurrentFramePos(pos);
//    DataCenter::getInstance()->changeRealAnd3DData(getCurrentFrameIndex(),500);
}

int FrameViewer::getFrameAtCurrentPos()
{
    auto frame_distance = frameDistance();
    auto pos = mapFromGlobal(QCursor::pos());
    int idx = qRound(pos.x() / frame_distance);
    if (idx < 0) {
        idx = 0;
    }
    return idx;
}

void FrameViewer::setFrameNumber(int fn)
{
    frameNum_ = fn;
    if (frameAxis_) {
        frameAxis_->setMaxValue(frameNum_);
    }
    if (timeAxis_) {
        timeAxis_->setMaxValue(static_cast<double>(frameNum_) / frameRate_);
    }
    if (behaviorLayerContainer_) {
        behaviorLayerContainer_->onFrameNumberChanged(fn);
    }
    setFixedWidth(frameAxis_->width());
}

QRect FrameViewer::getBehaviorLayerRect()
{

    auto pos = mapToGlobal(behaviorLayerContainer_->pos());
    QRect rect(pos.x(),pos.y(),behaviorLayerContainer_->width(),behaviorLayerContainer_->height());
    return rect;
}

bool FrameViewer::eventFilter(QObject *w, QEvent *e)
{
    if (auto we = dynamic_cast<QWheelEvent*>(e)) {
        if (w == behaviorLayerContainerScrollArea_->viewport()
                && (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
            frameOnCursorWhileScaling_ = getFrameAtCurrentPos();
            if (we->angleDelta().y() > 0) {
                emit wheelWhileCtrlPressing(true);
            } else if (we->angleDelta().y() < 0) {
                emit wheelWhileCtrlPressing(false);
            }
            e->accept();
            return true;
        }
    }
    return false;
}

void FrameViewer::resizeEvent(QResizeEvent * /*e*/)
{
    setFixedWidth(frameAxis_->width());
//    currentFramePointer_->setFixedHeight(height());
    framePointerLine_->setFixedHeight(height()-framePointerTop_->height()-30);
    moveFramePointer();
}

void FrameViewer::addFileToCurrentFrame(const QString &filePath)
{
    if (behaviorLayerContainer_ && framePointerTop_) {
        behaviorLayerContainer_->addFileToFrame(framePointerTop_->getFrameIndex(), filePath);
    }
}

double FrameViewer::frameDistance() const
{
    return frameDistanceBase_ * scaleFactor_;
}

void FrameViewer::addMusic(const QString& filePath)
{
    if (behaviorLayerContainer_) {
        behaviorLayerContainer_->addMusic(filePath);
    }
}

void FrameViewer::onFrameDivisionChanged(int frameDivision)
{
    frameRate_ = 1000/frameDivision;
    double topTickInterval = frameAxis_->getTickInterval();
    double topTickDistance = frameAxis_->tickDistance();
    double bottomTickDistance = topTickDistance * 5;
    double bottomTickInterval = (topTickInterval * bottomTickDistance) / (topTickDistance * frameRate_);
    if (timeAxis_) {
        timeAxis_->setFixedWidth(frameAxis_->width());
        timeAxis_->changeTick(bottomTickInterval, bottomTickDistance);
    }

    behaviorLayerContainer_->onDivisionChanged();
}

void FrameViewer::moveFramePointer()
{
//    currentFramePointer_->move(QPoint(currentFramePointer_->getFrameIndex() * frameDistance() - currentFramePointer_->width() / 2, 0));
    framePointerTop_->move(QPoint(framePointerTop_->getFrameIndex() * frameDistance() - framePointerTop_->width() / 2, 2));
    framePointerLine_->move(framePointerTop_->getFrameIndex() * frameDistance()-1,framePointerTop_->height()-5);
    framePointerBottom_->move(framePointerTop_->getFrameIndex() * frameDistance() - framePointerBottom_->width() / 2,framePointerLine_->y()+framePointerLine_->height()+2);
    int division = UBTDataCenter->frameDivision();
    int frameIndex = framePointerTop_->getFrameIndex();
    framePointerBottom_->setText(QString("%1s").arg((int)frameIndex*division/1000));
}

void FrameViewer::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QWidget::paintEvent(event);
}
