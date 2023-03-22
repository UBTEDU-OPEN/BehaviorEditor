#include "framepointertop.h"
#include "ui_framepointertop.h"

const QString kTextFormat("%1f");

FramePointerTop::FramePointerTop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FramePointerTop)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
}

FramePointerTop::~FramePointerTop()
{
    delete ui;
}

void FramePointerTop::setFrameIndex(int fi)
{
    frameIndex_ = fi;
    QString text = kTextFormat.arg(fi);
    ui->pos->setText(text);
}
