#ifndef FRAMEVIEWERCONTAINER_H
#define FRAMEVIEWERCONTAINER_H

#include <QWidget>
#include <QKeyEvent>
#include <QPushButton>
#include <QVector>
#include <QMap>
#include <QPair>
#include <QTimer>
#include <QScrollBar>
#include <QScrollArea>

#include "axis.h"
#include "behaviorlayercontainer.h"
#include "framepointer.h"
#include "frameviewer.h"
#include "labelblock.h"

class FrameViewerContainer : public QWidget
{
    Q_OBJECT

public:
    explicit FrameViewerContainer(int fn = 2000, double fd = 50, int fps = 20, QWidget *parent = nullptr);
    ~FrameViewerContainer() override;

    void onChangeFrameIndex(int frameIndex);
    void setScaleValue(int scale);
    void onHorizantolScrolled(int value);
    void onHorizantolScrollBarRangeChanged(int min, int max);
    void onVScrollBarValueChanged(int value);
    void onBehaviorLayerContainerVScrollBarValueChanged(double value);
    void onBehaviorVScrollBarRangeChanged(int min, int max);

    void addFileToCurrentFrame(const QString &filePath);
    void addMusic(const QString&);
    void changeFrameInterval(int);
    void onFrameDivisionChanged(int);

    void setCurrentFrameIndex(int fi);
    QRect getBehaviorLayerRect();

protected:
    void resizeEvent(QResizeEvent *e) override;
    void keyPressEvent(QKeyEvent* e) override;

private:
    bool slideByFrameViewerContainerVScrollBar() const;
    void configStyle();

signals:
    void sigKeyEvent(QKeyEvent*);
    void behaviorLayerContainerVScrollBarValueChanged(double v);
    void frameViewerContainerVScrollBarValueChanged(double v);
    void currentFrameIndexChanged(int frameIndex, QMap<int, QPair<double, int>> servoInfo, bool isKeyFrame);
//    void nextKeyFrame(int frameIndex, QMap<int, QPair<double, int>> servoInfo);
    void wheelWhileCtrlPressing(bool up);
    void zoomIn();
    void zoomOut();

private:
    QScrollArea *frameContainer_;
    FrameViewer *frameViewer_;
    QScrollBar *frameViewerContainerVScrollBar_;
};

#endif // FRAMEVIEWERCONTAINER_H
