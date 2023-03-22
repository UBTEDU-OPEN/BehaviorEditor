#ifndef FRAMEVIEWER_H
#define FRAMEVIEWER_H

#include <QWidget>
#include <QMap>
#include <QPair>

#include <QScrollArea>
#include <QTimer>
#include <QLabel>

#include "axis.h"
#include "behaviorlayercontainer.h"
#include "framepointer.h"
#include "datacenter.h"
#include "framepointertop.h"

class FrameViewer : public QWidget
{
    Q_OBJECT
public:
    FrameViewer(int fn, double fd, int fps, QWidget *parent = nullptr);
    ~FrameViewer() override;

    bool onNewBehavior();
    void onChangeFrameIndex(int frameIndex);
    int getFrameNumber() const { return frameNum_; }
    double frameDistance() const;
    int getFrameOnCursorWhileScaling() const { return frameOnCursorWhileScaling_; }

    void setScaleValue(int scale);
    void onHorizantolScrolled(double value);
    void onFrameViewerContainerVScrollBarValueChanged(double value);
    void onBehaviorLayerContainerVScrollBarValueChanged(int value);
    void onBehaviorVScrollBarRangeChanged(int min, int max);
    void onActLayerTreeScrolled(double v);
    void onKeyEvent(QKeyEvent*);

    void addFileToCurrentFrame(const QString &filePath);

    void addMusic(const QString&);
    void onFrameDivisionChanged(int);

    void setCurrentFrameIndex(int fi);
    int getCurrentFrameIndex() const;
    void setCurrentFramePos(double pos);
    void setCurrentFramePosAndSendToRobot(double pos);
    int getFrameAtCurrentPos();
    void setFrameNumber(int fn);

    QRect getBehaviorLayerRect();

    bool eventFilter(QObject *w, QEvent *e) override;

protected:
    void resizeEvent(QResizeEvent *e) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void onPlayerTimerTimeout();
    void onPlayMusicTimeOut();
    void moveFramePointer();

signals:
    void behaviorLayerContainerVScrollBarValueChanged(double v);
    void behaviorVScrollBarRangeChanged(int min, int max);
    void currentFrameIndexChanged(int frameIndex, QMap<int, QPair<double, int>> servoInfo, bool isKeyFrame);
    void wheelWhileCtrlPressing(bool up);
    void zoomIn();
    void zoomOut();

private:
    Axis *frameAxis_;
    Axis *timeAxis_;
    QScrollArea *behaviorLayerContainerScrollArea_;
    BehaviorLayerContainer *behaviorLayerContainer_;
    int frameOnCursorWhileScaling_;

    double scaleFactor_;
    int frameNum_;
    double frameDistanceBase_;
    int frameRate_;
    int currentFrameIndex_;
    FramePointerTop* framePointerTop_;
    QLabel* framePointerLine_;
    QLabel* framePointerBottom_;
};

#endif // FRAMEVIEWER_H
