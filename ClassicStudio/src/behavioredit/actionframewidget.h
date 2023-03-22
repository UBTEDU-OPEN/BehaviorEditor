#ifndef ACTIONFRAMEWIDGET_H
#define ACTIONFRAMEWIDGET_H

#include <QWidget>

namespace Ui {
class ActionFrameWidget;
}

class ActionFrameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActionFrameWidget(QWidget *parent = nullptr);
    ~ActionFrameWidget();
    void setFrameSelected(bool);
    void setFrameIndex(int frameIndex) { frameIndex_ = frameIndex; }
    int frameIndex() { return frameIndex_; }

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    Ui::ActionFrameWidget *ui;
    bool selected_ = false;
    int frameIndex_ = -1;
};

#endif // ACTIONFRAMEWIDGET_H
