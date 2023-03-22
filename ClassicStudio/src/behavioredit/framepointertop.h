#ifndef FRAMEPOINTERTOP_H
#define FRAMEPOINTERTOP_H

#include <QWidget>

namespace Ui {
class FramePointerTop;
}

class FramePointerTop : public QWidget
{
    Q_OBJECT

public:
    explicit FramePointerTop(QWidget *parent = nullptr);
    ~FramePointerTop();

    void setFrameIndex(int fi);
    int getFrameIndex() const { return frameIndex_; }

private:
    Ui::FramePointerTop *ui;
    int frameIndex_ = 1;
};

#endif // FRAMEPOINTERTOP_H
