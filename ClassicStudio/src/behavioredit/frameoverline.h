#ifndef FRAMEOVERLINE_H
#define FRAMEOVERLINE_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>

class FrameOverLine : public QWidget
{
    Q_OBJECT
public:
    explicit FrameOverLine(QWidget *parent = nullptr);

    int getFrameIndex() const {return frameIndex_;}
    void setFrameIndex(int frameIndex) {frameIndex_ = frameIndex;}

protected:
    void paintEvent(QPaintEvent *e) override;
    void mouseMoveEvent(QMouseEvent* e) override;

signals:
    void sigDragFrameOverLine(QPoint);

private:
    int frameIndex_;

};

#endif // FRAMEOVERLINE_H
