#include "labelblock.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QFont>

#include "labelcontenteditor.h"
#include "datacenter.h"

int LabelBlock::sIndex = kStaticLabelIndex;

LabelBlock::LabelBlock(QWidget *parent) :
    QWidget(parent),
    index_(sIndex),
    frameIndex_(-1)
{
    setStyleSheet("LabelBlock{background-color: rgb(255, 255, 0);}");
    if (index_ != kStaticLabelIndex) {
        content_ = QString::number(index_);
        menu_ = new QMenu(this);
        edit_ = new QAction(this);
        edit_->setText(tr("Edit"));
        delete_ = new QAction(this);
        delete_->setText(tr("Delete"));
        menu_->addAction(edit_);
        menu_->addSeparator();
        menu_->addAction(delete_);
        connect(edit_,&QAction::triggered,this,&LabelBlock::onEdit);
        connect(delete_,&QAction::triggered,this,&LabelBlock::onDelete);
    }
    sIndex++;
}


void LabelBlock::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton && index_ != kStaticLabelIndex) {
        menu_->move(e->globalPos());
        menu_->show();
    }

    //QWidget::mousePressEvent(e);
}

void LabelBlock::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton && index_ != kStaticLabelIndex) {
        openEditDialog();
    }
}

void LabelBlock::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons().testFlag(Qt::LeftButton)) {
        emit sigMouseMove(index_,e->globalPos());
    }
}

void LabelBlock::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        emit sigMouseRelease(index_,e->globalPos());
    }
}


void LabelBlock::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.save();
    painter.fillRect(e->rect(), QBrush(Qt::yellow));
    if (!content_.isEmpty()) {
        QFont font;
        font.setPointSize(10);
        painter.setFont(font);
        QPoint pos{0,height()-1};
        painter.drawText(pos,content_);
    }
    painter.restore();
}

void LabelBlock::openEditDialog()
{
    LabelContentEditor editor(frameIndex_,DataCenter::getInstance()->frameRate(),content_);
    connect(&editor,&LabelContentEditor::sigContentChanged,this,&LabelBlock::onContentChanged);
    editor.exec();
}

void LabelBlock::onContentChanged(int frameIndex,QString label)
{
    setContent(label);
    if (frameIndex != frameIndex_) {
        frameIndex_ = frameIndex;
        emit sigFrameIndexChange(index_,frameIndex_);
    }
}

void LabelBlock::onEdit()
{
    openEditDialog();
}

void LabelBlock::onDelete()
{
    emit sigDelete(index_);
}

