#ifndef BEHAVIOREDIT_H
#define BEHAVIOREDIT_H

#include <QWidget>
#include <QMap>
#include <QPair>
#include <QHBoxLayout>

#include "behavioreditglobal.h"
#include "controlwidget.h"
#include "frameviewercontainer.h"

class BEHAVIOREDIT_EXPORT BehaviorEdit : public QWidget
{
    Q_OBJECT

private:
    const static QString kQssPath;

public:
    BehaviorEdit(QWidget *parent = nullptr);
    ~BehaviorEdit() override;
    QRect getBehaviorLayerRect();

public slots:
    void addFileToCurrentFrame(const QString &filePath);
    void addMusic(const QString&);
    void onChangeFrameIndex(int frameIndex);
    void onTotalTimeChanged(int secs);
    void onFrameDivisionChanged(int);
    void onZoomIn();
    void onZoomOut();

protected:
    void setDefaultStyle();

private:
    QHBoxLayout *mainLayout_;
    ControlWidget *controlWidget_;
    FrameViewerContainer *frameViewerContainer_;
};
#endif // BEHAVIOREDIT_H
