#include "musicwidget.h"
#include "ui_musicwidget.h"

#include <QPaintEvent>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QFileInfo>

#include "datacenter.h"

MusicWidget::MusicWidget(const QString& filePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicWidget)
{
    ui->setupUi(this);
    ui->audioItem->setFilePath(filePath);
    setProperty("selected",true);
    QFileInfo info(filePath);
    ui->label->setText(info.baseName());
    setToolTip(info.fileName());
    UBTDataCenter->selectMusic();
    audioMenu_ = new QMenu(this);
    auto deleteAction = new QAction(tr("Delete"),this);
    audioMenu_->addAction(deleteAction);
    connect(deleteAction,&QAction::triggered,[this]{
        emit deleteMusic(ui->label->text());
    });
}

MusicWidget::~MusicWidget()
{
    delete ui;
}

AudioItem* MusicWidget::getAudioItem()
{
    return ui->audioItem;
}

void MusicWidget::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QWidget::paintEvent(event);
}

void MusicWidget::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton) {
        setSelected(true);
        UBTDataCenter->selectMusic();
    } else if(event->button() == Qt::RightButton) {
        audioMenu_->move(QCursor::pos());
        audioMenu_->show();
    }
}

void MusicWidget::setSelected(bool selected)
{
    setProperty("selected",selected);
    style()->unpolish(this);
    style()->polish(this);
    emit musicSelected(ui->label->text(),selected);
}
