#ifndef FRAMEVIEWPLACEHOLDER_H
#define FRAMEVIEWPLACEHOLDER_H

#include <QWidget>

namespace Ui {
class FrameViewPlaceHolder;
}

class FrameViewPlaceHolder : public QWidget
{
    Q_OBJECT

public:
    explicit FrameViewPlaceHolder(QWidget *parent = nullptr);
    ~FrameViewPlaceHolder();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Ui::FrameViewPlaceHolder *ui;
};

#endif // FRAMEVIEWPLACEHOLDER_H
