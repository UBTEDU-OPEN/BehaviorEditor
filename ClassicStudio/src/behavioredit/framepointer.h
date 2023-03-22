#ifndef FRAMEPOINTER_H
#define FRAMEPOINTER_H

#include <QWidget>

class FramePointer : public QWidget
{
    Q_OBJECT

public:
    static const int WIDTH = 30;
    static const int LABEL_HEIGHT = 15;
    static const int CIRCLE_DIAMETER = 20;
    static const QRect dragArea;

public:
    explicit FramePointer(QWidget *parent = nullptr);
    ~FramePointer();

    void setFrameIndex(int fi);
    int getFrameIndex() const {return frameIndex_; }
    void setFrameRate(int fr);
    QRect getDragArea() const;

signals:
    void frameDoubleClicked(int);

protected:
    void paintEvent(QPaintEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;

private:
    int frameIndex_ = 1;
    int frameRate_ = 0;
};

#endif // FRAMEPOINTER_H
